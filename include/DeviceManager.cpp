#include "DeviceManager.h"
#define NOMINMAX
//DEFINITION_SINGLE(CDeviceManager);

CDeviceManager::CDeviceManager()
{
}

CDeviceManager::~CDeviceManager()
{
	//SAFE_RELEASE(mRenderTargetView);
	//SAFE_RELEASE(mDepthStencilView);
	//SAFE_RELEASE(mDevice);
	//SAFE_RELEASE(mContext);
	//if(mContext)
	//	mContext->ClearState();
	//SAFE_RELEASE(mSwapChain);

	if (mContext)
	{
		// ID3D11DeviceContext::ClearState: 파이프라인의 모든 상태(셰이더, 정점 버퍼 등)를 기본값으로 재설정
		// ID3D11DeviceContext::Flush: 큐에 쌓인 GPU 명령을 즉시 제출하여 미해결된 리소스 참조를 강제로 해제
		mContext->ClearState();
		mContext->Flush();
	}

	for (int i = 0; i < 3; ++i)
	{
		mMSAATexture[i].Reset();
		mMSAARenderTargetView[i].Reset();
		mMSAAShaderResourceView[i].Reset();
	}
	mDepthStencilView.Reset();
	mDepthSRV.Reset();
	
	for (int i = 0; i < 2; ++i)
	{
		mPostProcessTextures[i].Reset();
		mPostProcessRTVs[i].Reset();
		mPostProcessSRVs[i].Reset();
	}
	mRenderTargetView.Reset();

	mResolvedHDRTexture.Reset();
	mResolvedHDRSRV.Reset();

	mLightAccumTextureMSAA.Reset();
	mLightAccumRTVMSAA.Reset();
	
	mSwapChain.Reset();
	mContext.Reset();
	mDevice.Reset();
}

bool CDeviceManager::init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode)
{
	mhWnd = hWnd;
	mResolution.width = width;
	mResolution.height = height;
	mWindowMode = windowMode;

	if (!createDevice()) return false;
	if (!createSwapChain(windowMode)) return false;
	if (!createDepthStencilView()) return false;
	if (!createGeometryBuffers()) return false;
	if (!createLightingBuffers()) return false;
	if (!createPostProcessBuffers()) return false;
	if (!createFullQuadBuffer()) return false;

	setViewport();
	
	return true;
}

bool CDeviceManager::createDevice()
{
	// D3D11_CREATE_DEVICE_FLAG 열거형에서 D3D11_CREATE_DEVICE_BGRA_SUPPORT 플래그를 사용
	unsigned int createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	// D3D_FEATURE_LEVEL 열거형에서 D3D_FEATURE_LEVEL_11_0 플래그를 사용하여 Direct3D 11.0 기능 레벨을 지정
	// D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, };
	// D3D_FEATURE_LEVEL 열거형에서 D3D_FEATURE_LEVEL_11_0 플래그를 사용하여 Direct3D 11.0 기능 레벨을 지정할 때 결과를 저장할 변수 선언
	D3D_FEATURE_LEVEL levelResult;

#ifdef DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

	// arg1 : nullptr 적용 시 기본 어댑터 사용 (직접 그래픽카드 선택 시 IDXGIAdapter 인터페이스 포인터를 전달)
	// arg2 : D3D_DRIVER_TYPE_HARDWARE 적용 시 하드웨어 가속 사용
	// arg3 : 소프트웨어 드라이버 사용 시 HMODULE 핸들 전달 (D3D_DRIVER_TYPE_SOFTWARE 적용 시)
	// arg4 : createDeviceFlags 플래그 전달
	// arg5 : 지원할 기능 레벨 배열 전달 (nullptr 적용 시 모든 기능 레벨 지원)
	// arg6 : 기능 레벨 배열의 요소 수 전달
	// arg7 : SDK 버전 전달 (D3D11_SDK_VERSION 매크로 사용)
	// arg8 : ID3D11Device 인터페이스 포인터의 주소 전달 (생성된 장치 객체 반환)
	// arg9 : 기능 레벨 결과 저장할 변수의 주소 전달
	// arg10 : ID3D11DeviceContext 인터페이스 포인터의 주소 전달 (생성된 장치 컨텍스트 객체 반환)
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, featureLevels, 2, D3D11_SDK_VERSION, &mDevice, &levelResult, &mContext)))
		return false;

	// 생성된 Device 객체의 MSAA 품질 수준을 확인하고 결과를 저장
	// DX에서는 0-1사이의 normal화 된 색상값을 사용
	// mSampleCount : 1(사용하지 않음) 또는 4(4배 멀티샘플링)로 설정하여 MSAA 품질 수준을 확인
	// mCheckColor : 해당 샘플 개수에서 MSAA 품질 수준이 지원되는지 여부를 저장하는 변수
	mSampleCount = 4;
	mCheckColor = 0;

	mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, mSampleCount, &mCheckColor);

	if (mCheckColor < 1)
		mSampleCount = 1;

	return true;
}

bool CDeviceManager::createSwapChain(bool windowMode)
{
	/* DXGI_SWAP_CHAIN_DESC 구조체를 초기화
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	// Backbuffer 설정
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 화면주사율 설정 : 주사율은 시스템 기본값 사용 (0/1)
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	// 화면	스케일링 설정 (Default 설정)
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// 스캔라인 순서 설정 (Default 설정)
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// 백버퍼 수 설정 (1로 설정하여 단일 백버퍼 사용) - 수정
	// Flip 방식은 최소 2개 이상의 버퍼가 필요
	swapChainDesc.BufferCount = 2;
	// 백버퍼 사용 용도 설정 (렌더 타겟 출력으로 설정)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 출력 창 핸들 설정
	swapChainDesc.OutputWindow = mhWnd;

	// DXGI_SWAP_EFFECT_DISCARD 방식
	// Multi-sampling 설정 (mSampleCount 값에 따라 4배 멀티샘플링 또는 사용하지 않음)
	// DX지원 Multisampling 속도가 낮으므로 NVIDIA FSAA 기능을 활용하는 것을 권장
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = mSampleCount;

	// DXGI_SWAP_EFFECT_FLIP_DISCARD 방식
	// DirectX의 Flip Model(FLIP_DISCARD, FLIP_SEQUENTIAL)은 운영체제가 버퍼를 직접 관리
	// Flip 방식 스왑체인은 MSAA를 지원하지 않으므로 별도의 RenderTargetView 필요 (반드시 1, 0)
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// 창 모드 설정
	swapChainDesc.Windowed = windowMode;

	// 스왑 효과 설정 (DXGI_SWAP_EFFECT_DISCARD로 설정하여 이전 프레임 버퍼 내용이 유지되지 않도록 함)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 스왑 효과 설정 (Flip Model 방식으로 설정하여 더 낮은 지연 시간과 향상된 성능 제공)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	// 창 모드/전체 화면 전환 시 더 부드러운 처리를 위한 플래그
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	*/

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = mResolution.width;
	swapChainDesc.Height = mResolution.height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc.Count = 1;      // Flip 모델은 무조건 1
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;           // Flip 모델은 최소 2
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//전체화면 정보는 별도 구조체로 분리
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc = {};
	fsDesc.RefreshRate.Numerator = 0;
	fsDesc.RefreshRate.Denominator = 1;
	fsDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	fsDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fsDesc.Windowed = windowMode;

	/* 스왑 체인 생성 */
	// IUnknown 인터페이스를 상속받는 객체들은 AddRef와 Release 메서드를 통해 참조 카운트를 관리
	// type에 맞는 iid를 얻어오는 __uuidof 연산자를 사용하여 
	// mDevice 객체에서 IDXGIDevice 인터페이스 포인터를 얻어와 dxgiDevice 변수에 저장
	// IDXGIDevice* dxgiDevice = nullptr;
	ComPtr<IDXGIDevice> dxgiDevice;
	// mDevice->QueryInterface(__uuidof(IDXGIDevice),(void**)&dxgiDevice);
	// ComPtr.As() : ComPtr 객체가 관리하는 QueryInterface 호출을 래핑하여 지정된 인터페이스로 포인터를 반환하는 메서드
	mDevice.As(&dxgiDevice);

	// IDXGIDevice 인터페이스 포인터를 사용하여 IDXGIAdapter 인터페이스 포인터를 얻어와 dxgiAdapter 변수에 저장
	// IDXGIAdapter* dxgiAdapter = nullptr;
	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	/*
	* // IDXGIAdapter 인터페이스 포인터를 사용하여 IDXGIFactory 인터페이스 포인터를 얻어와 dxgiFactory 변수에 저장
	* // IDXGIFactory* dxgiFactory = nullptr;
	* ComPtr<IDXGIFactory> dxgiFactory;
	* dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	*/

	ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);

	if (FAILED(dxgiFactory->CreateSwapChainForHwnd(mDevice.Get(), mhWnd, &swapChainDesc, &fsDesc, nullptr, mSwapChain.GetAddressOf())))
		return false;

	/*
	// IDXGIFactory 인터페이스 포인터의 CreateSwapChain 메서드를 호출하여 스왑 체인 생성
	if (FAILED(dxgiFactory->CreateSwapChain(mDevice.Get(), &swapChainDesc, mSwapChain.GetAddressOf())))
	{
		//SAFE_RELEASE(dxgiFactory);
		//SAFE_RELEASE(dxgiAdapter);
		//SAFE_RELEASE(dxgiDevice);
		return false;
	}

	// 사용 종료 시 각 인터페이스 포인터의 Release 메서드를 호출하여 참조 카운트 제어
	//SAFE_RELEASE(dxgiFactory);
	//SAFE_RELEASE(dxgiAdapter);
	//SAFE_RELEASE(dxgiDevice);
	*/

	/* 렌더 타겟 뷰 생성 */
	// swapChain으로 생성한 BackBuffer를 렌더 타겟 뷰로 사용하기 위해 ID3D11Texture2D 인터페이스 포인터에 저장
	//ID3D11Texture2D* backBuffer = nullptr;
	ComPtr<ID3D11Texture2D> backBuffer;

	// Getbuffer 사용 시 AddRef가 호출되어 참조 카운트가 증가
	if (FAILED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
	{
		//SAFE_RELEASE(backBuffer);
		return false;
	}

	// backBuffer에 출력하기 위한 렌더 타겟 뷰 생성
	if (FAILED(mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &mRenderTargetView)))
	{
		//SAFE_RELEASE(backBuffer);
		return false;
	}

	//SAFE_RELEASE(backBuffer);
	return true;
}

bool CDeviceManager::createDepthStencilView()
{
	/* DepthStencilView Buffer로 사용할 텍스처 설정 D3D11_TEXTURE2D_DESC 구조체 초기화 */
	D3D11_TEXTURE2D_DESC depthDesc = {};

	depthDesc.Width = mResolution.width;
	depthDesc.Height = mResolution.height;
	depthDesc.ArraySize = 1;
	// MipLevels는 텍스처의 MIP 맵 레벨 수를 지정하는 멤버
	// Depth Buffer는 화면과 1:1로 대응되어 축소된 이미지가 필요하지 않으므로 MipLevels를 1로 설정하여 단일 레벨의 텍스처를 사용하도록 함
	depthDesc.MipLevels = 1;
	// GPU에서 해당 리소스를 읽고쓰기를 모두 수행한다는	것을 알리기 위해 D3D11_USAGE_DEFAULT로 설정
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.SampleDesc.Count = mSampleCount;
	// Default 설정
	depthDesc.SampleDesc.Quality = (std::max)(0, static_cast<int>(mCheckColor) - 1);
	// TYPELESS로 생성하면 하나의 리소스를 용도에 따라 다른 관점(View)으로 해석
	// DXGI_FORMAT_D24_UNORM_S8_UINT :Depth Stencil View 생성 시 사용
	depthDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	// 그래픽카드에 해당 리소스가 DepthStencilView로 사용될 것임을 알리기 위해 D3D11_BIND_DEPTH_STENCIL 플래그 설정
	// DSV와 SRV 둘 다 쓰겠다고 선언
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	/* DepthStencil Buffer로 사용할 텍스처 생성 */
	//ID3D11Texture2D* depthStencilBuffer = nullptr;
	ComPtr<ID3D11Texture2D> depthStencilBuffer;

	if (FAILED(mDevice->CreateTexture2D(&depthDesc, nullptr, &depthStencilBuffer)))
	{
		//SAFE_RELEASE(depthStencilBuffer);
		return false;
	}

	// DSV 생성 (해석: Depth/Stencil)
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	// 실제 깊이 포맷
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
	// MSAA 여부에 따라 디멘션 설정
	dsvDesc.ViewDimension = (mSampleCount > 1) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;

	/* DepthStencilView 생성 */
	if (FAILED(mDevice->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, &mDepthStencilView)))
	{
		//SAFE_RELEASE(depthStencilBuffer);
		return false;
	}

	//SRV 생성 (해석: Shader에서 읽기용)
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	// 24비트 R채널(Depth)만 R로 읽음
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = (mSampleCount > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;

	if (FAILED(mDevice->CreateShaderResourceView(depthStencilBuffer.Get(), &srvDesc, mDepthSRV.GetAddressOf())))
		return false;

	//SAFE_RELEASE(depthStencilBuffer);
	return true;
}

bool CDeviceManager::createGeometryBuffers()
{
	/* MSAA를 적용하기 위한 Render Target View 용 텍스처 설정 구조체 초기화 */
	// RTV배열 기본값
	D3D11_TEXTURE2D_DESC msaaDesc = {};
	msaaDesc.Width = mResolution.width;
	msaaDesc.Height = mResolution.height;
	msaaDesc.MipLevels = 1;
	msaaDesc.ArraySize = 1;
	msaaDesc.SampleDesc.Count = mSampleCount;
	msaaDesc.SampleDesc.Quality = (std::max)(0, static_cast<int>(mCheckColor) - 1);
	msaaDesc.Usage = D3D11_USAGE_DEFAULT;
	// 라이팅 단계에서 입력으로 써야 하므로 SHADER_RESOURCE를 반드시 추가
	msaaDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	// --- 1. 색상용 텍스처 설정 ---
	D3D11_TEXTURE2D_DESC colorDesc = msaaDesc; // 동일한 크기, 동일한 MSAA 설정 복사
	colorDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 표준 색상 포맷

	// --- 2. 법선용 텍스처 설정 ---
	D3D11_TEXTURE2D_DESC normalDesc = msaaDesc; // 동일한 크기, 동일한 MSAA 설정 복사
	normalDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // 고정밀도 부동소수점 포맷

	// --- 3. 스펙큘러용 텍스처 설정 ---
	D3D11_TEXTURE2D_DESC specularDesc = msaaDesc;
	specularDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 재질 수치는 0-1

	if (FAILED(mDevice->CreateTexture2D(&colorDesc, nullptr, mMSAATexture[0].GetAddressOf())))
		return false;
	if (FAILED(mDevice->CreateRenderTargetView(mMSAATexture[0].Get(), nullptr, mMSAARenderTargetView[0].GetAddressOf())))
		return false;

	if (FAILED(mDevice->CreateTexture2D(&normalDesc, nullptr, mMSAATexture[1].GetAddressOf())))
		return false;
	if (FAILED(mDevice->CreateRenderTargetView(mMSAATexture[1].Get(), nullptr, mMSAARenderTargetView[1].GetAddressOf())))
		return false;

	if (FAILED(mDevice->CreateTexture2D(&specularDesc, nullptr, mMSAATexture[2].GetAddressOf())))
		return false;
	if (FAILED(mDevice->CreateRenderTargetView(mMSAATexture[2].Get(), nullptr, mMSAARenderTargetView[2].GetAddressOf())))
		return false;

	/* ShaderResoureView 생성 (nullptr 전달 시 텍스처 설정을 기반으로 기본 뷰 생성) */
	for (int i = 0; i < 3; ++i)
		if (FAILED(mDevice->CreateShaderResourceView(mMSAATexture[i].Get(), nullptr, mMSAAShaderResourceView[i].GetAddressOf())))
			return false;

	return true;
}

bool CDeviceManager::createLightingBuffers()
{
	/* --- Light Accumulation Buffer (MSAA) 생성 --- */
	D3D11_TEXTURE2D_DESC lightDesc = {};
	lightDesc.Width = mResolution.width;
	lightDesc.Height = mResolution.height;
	lightDesc.MipLevels = 1;
	lightDesc.ArraySize = 1;
	// GBuffer와 동일한 MSAA 설정 사용
	lightDesc.SampleDesc.Count = mSampleCount;
	lightDesc.SampleDesc.Quality = (std::max)(0, static_cast<int>(mCheckColor) - 1);
	// 라이팅은 HDR 처리를 위해 R16G16B16A16_FLOAT 권장
	// (0.0~1.0 범위를 넘어서는 밝기 값을 보존하기 위함)
	lightDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	lightDesc.Usage = D3D11_USAGE_DEFAULT;
	// RTV로 그려야 하고, 나중에 필요하면 SRV로도 쓸 수 있게 설정
	lightDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	// 텍스처 생성
	if (FAILED(mDevice->CreateTexture2D(&lightDesc, nullptr, mLightAccumTextureMSAA.GetAddressOf())))
		return false;

	// 렌더 타겟 뷰 생성
	if (FAILED(mDevice->CreateRenderTargetView(mLightAccumTextureMSAA.Get(), nullptr, mLightAccumRTVMSAA.GetAddressOf())))
		return false;

	/* --- Resolved HDR Buffer (Non-MSAA) 생성 --- */
	D3D11_TEXTURE2D_DESC resDesc = {};
	resDesc.Width = mResolution.width;
	resDesc.Height = mResolution.height;
	resDesc.MipLevels = 1;
	resDesc.ArraySize = 1;
	// 포스트 프로세스를 위해 샘플 수는 무조건 1개 (Resolve의 목적)
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	// 라이팅 버퍼와 동일한 HDR 포맷 유지
	resDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	resDesc.Usage = D3D11_USAGE_DEFAULT;
	// 셰이더에서 읽어야 하므로 SHADER_RESOURCE가 필수
	resDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	// 텍스처 생성
	if (FAILED(mDevice->CreateTexture2D(&resDesc, nullptr, mResolvedHDRTexture.GetAddressOf())))
		return false;

	// 셰이더 리소스 뷰(SRV) 생성 (포스트 프로세스 셰이더의 입력값으로 쓰기 위함)
	if (FAILED(mDevice->CreateShaderResourceView(mResolvedHDRTexture.Get(), nullptr, mResolvedHDRSRV.GetAddressOf())))
		return false;

	return true;
}

bool CDeviceManager::createPostProcessBuffers()
{
	/* --- Post-Process Ping-Pong Buffers (Non-MSAA, HDR) 생성 --- */
	D3D11_TEXTURE2D_DESC postDesc = {};
	postDesc.Width = mResolution.width;
	postDesc.Height = mResolution.height;
	postDesc.MipLevels = 1;
	postDesc.ArraySize = 1;
	// 포스트 프로세스는 Resolve 이후에 진행되므로 샘플 수는 1
	postDesc.SampleDesc.Count = 1;
	postDesc.SampleDesc.Quality = 0;
	// HDR 연산을 위해 고정밀도 포맷 유지
	postDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	postDesc.Usage = D3D11_USAGE_DEFAULT;
	// 렌더 타겟(쓰기)이자 셰이더 리소스(읽기)로 모두 사용
	postDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	for (int i = 0; i < 2; ++i)
	{
		// 텍스처 생성
		if (FAILED(mDevice->CreateTexture2D(&postDesc, nullptr, mPostProcessTextures[i].GetAddressOf())))
			return false;

		// 렌더 타겟 뷰(RTV) 생성 - 효과를 그려넣는 용도
		if (FAILED(mDevice->CreateRenderTargetView(mPostProcessTextures[i].Get(), nullptr, mPostProcessRTVs[i].GetAddressOf())))
			return false;

		// 셰이더 리소스 뷰(SRV) 생성 - 다음 효과의 입력으로 쓰는 용도
		if (FAILED(mDevice->CreateShaderResourceView(mPostProcessTextures[i].Get(), nullptr, mPostProcessSRVs[i].GetAddressOf())))
			return false;
	}

	return true;
}

void CDeviceManager::setViewport()
{
	/* Viewport 생성용 구조체 초기화 */
	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(mResolution.width);
	viewport.Height = static_cast<float>(mResolution.height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// 렌더링할 영역을 설정하기 위해 생성한 Viewport를 장치 컨텍스트에 설정
	mContext->RSSetViewports(1, &viewport);
}

bool CDeviceManager::createFullQuadBuffer()
{
	// 화면 전체를 덮는 거대 삼각형 (정점 3개)
	// NDC: (-1, 3), (3, -1), (-1, -1)
	// UV:  (0, -1), (2, 1), (0, 1)
	FPostProcessVertex vertices[3];

	vertices[0].pos = DirectX::XMFLOAT3(-1.0f, 3.0f, 0.0f);
	vertices[0].uv = DirectX::XMFLOAT2(0.0f, -1.0f);

	vertices[1].pos = DirectX::XMFLOAT3(3.0f, -1.0f, 0.0f);
	vertices[1].uv = DirectX::XMFLOAT2(2.0f, 1.0f);

	vertices[2].pos = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[2].uv = DirectX::XMFLOAT2(0.0f, 1.0f);

	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE; // 수정할 필요 없으므로 최적화용 Immutable
	vbd.ByteWidth = sizeof(FPostProcessVertex) * 3;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vinit = {};
	vinit.pSysMem = vertices;

	if (FAILED(mDevice->CreateBuffer(&vbd, &vinit, mFullQuadVB.GetAddressOf())))
		return false;

	return true;
}

void CDeviceManager::clearBuffers(const FLOAT clearColor[4])
{
	// GBuffer 클리어
	for (int i = 0; i < 3; ++i)
		if (mMSAARenderTargetView[i])
			mContext->ClearRenderTargetView(mMSAARenderTargetView[i].Get(), clearColor);

	// 라이팅 버퍼 클리어 (HDR이므로 보통 검은색 0,0,0,0으로 제거)
	float black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	mContext->ClearRenderTargetView(mLightAccumRTVMSAA.Get(), black);

	// 깊이 버퍼 클리어
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void CDeviceManager::BeginGeometryPass()
{
	unbindShaderResources();

	// 렌더 타겟과 깊이 스텐실 뷰를 장치 컨텍스트에 설정하여 렌더링 준비
	// OMSetRenderTargets는 ID3D11RenderTargetView*의 배열을 인자로 전달받음.
	// ComPtr 배열을 직접 넣을 수 없으므로, 원시 포인터 배열화 후 전달
	ID3D11RenderTargetView* targets[3] = {
		mMSAARenderTargetView[0].Get(),
		mMSAARenderTargetView[1].Get(),
		mMSAARenderTargetView[2].Get()
	};
	
	// 출력(Output Merger) 단계에서 사용할 렌더 타겟 뷰 배열과 깊이-스텐실 뷰를 설정
	mContext->OMSetRenderTargets(3, targets, mDepthStencilView.Get());
}

void CDeviceManager::BeginLightingPass()
{
	// 이전 단계의 SRV들 해제
	unbindShaderResources(); 
	// 라이팅 결과는 mLightAccumRTVMSAA 하나에만 저장
	// 이때 G-Buffer의 SRV를 읽어야 하므로, 기존에 걸려있던 G-Buffer RTV는 자동으로 해제되어야 함
	mContext->OMSetRenderTargets(1, mLightAccumRTVMSAA.GetAddressOf(), nullptr);
}

void CDeviceManager::ResolveAndBeginPostProcess()
{ 
	// Resolve 대상들이 SRV로 꽂혀있을 가능성 제거
	unbindShaderResources();
	// RTV도 완전히 끊기
	ID3D11RenderTargetView* nullRTV[8] = {};
	mContext->OMSetRenderTargets(8, nullRTV, nullptr);

	// Resolve: MSAA HDR 라이팅 버퍼 -> 단일 샘플 HDR 텍스처로 병합
	// 이 과정이 끝나면 mResolvedHDRTexture에 MSAA가 풀린 HDR 이미지가 들어있게 됨
	mContext->ResolveSubresource(
		mResolvedHDRTexture.Get(), 0,           // 목적지
		mLightAccumTextureMSAA.Get(), 0,        // 소스
		DXGI_FORMAT_R16G16B16A16_FLOAT          // HDR 포맷 유지
	);
	// 이제부터는 mResolvedHDRSRV를 입력으로 쓰는 포스트 프로세스 셰이더들이 돌아갈 준비가 됨
}

void CDeviceManager::setPostProcessTarget(int index)
{
	// 핑퐁 시 이전 단계에서 SRV로 썼던 버퍼와의 충돌 방지
	unbindShaderResources(); 
	// 핑퐁 버퍼 중 하나를 출력 타겟으로 설정
	// 포스트 프로세스는 보통 깊이 판정이 필요 없으므로 DepthStencil은 nullptr로 설정
	mContext->OMSetRenderTargets(1, mPostProcessRTVs[index].GetAddressOf(), nullptr);
}

void CDeviceManager::setPostProcessSource(ID3D11ShaderResourceView* srv)
{
	// 픽셀 셰이더의 0번 슬롯(t0)에 소스 이미지(SRV)를 꽂음
	mContext->PSSetShaderResources(0, 1, &srv);
}

void CDeviceManager::BeginFinalPass()
{
	unbindShaderResources();
	// 최종 목적지인 스왑체인 백버퍼를 출력 타겟으로 설정
	// 포스트 프로세스 이후이므로 Depth는 필요 없음 (nullptr)
	mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr);

	/* 
	// 필요 시 뷰포트 재설정(혹시 다른 패스에서 바꿨을 수 있으므로 안전하게 다시 세팅)
	D3D11_VIEWPORT vp = {};
	vp.Width = (float)mResolution.width;
	vp.Height = (float)mResolution.height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	mContext->RSSetViewports(1, &vp);
	*/
}

void CDeviceManager::drawFullScreenQuad()
{
	// 버퍼 설정
	UINT stride = sizeof(FPostProcessVertex);
	UINT offset = 0;
	mContext->IASetVertexBuffers(0, 1, mFullQuadVB.GetAddressOf(), &stride, &offset);

	// 토폴로지 설정 (삼각형 리스트)
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 그리기 (정점 3개면 화면이 꽉 참)
	mContext->Draw(3, 0);
}

void CDeviceManager::present()
{
	// 수직 동기화(VSync)를 켠 상태로 화면 출력
	mSwapChain->Present(1, 0);
}


void CDeviceManager::unbindShaderResources()
{
	ID3D11ShaderResourceView* nullSRVs[16] = { nullptr };

	mContext->PSSetShaderResources(0, 16, nullSRVs);
	mContext->VSSetShaderResources(0, 16, nullSRVs);
	mContext->GSSetShaderResources(0, 16, nullSRVs);
	mContext->CSSetShaderResources(0, 16, nullSRVs);
}

void CDeviceManager::forwardRender()
{
	ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	
	if (SUCCEEDED(hr))
	{
		mContext->ResolveSubresource(
			backBuffer.Get(), 0,
			mMSAATexture[0].Get(), 0,
			DXGI_FORMAT_R8G8B8A8_UNORM
		);
	}

	mSwapChain->Present(1, 0);
}

DirectX::XMFLOAT2 CDeviceManager::getResolutionRatio() const
{
	RECT WindowRC;

	GetClientRect(mhWnd, &WindowRC);

	float Width = (float)WindowRC.right - WindowRC.left;
	float Height = (float)WindowRC.bottom - WindowRC.top;

	return DirectX::XMFLOAT2(mResolution.width / Width, mResolution.height / Height);
}
