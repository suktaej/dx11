#include "DeviceManager.h"

DEFINITION_SINGLE(CDeviceManager);

CDeviceManager::CDeviceManager()
{
}

CDeviceManager::~CDeviceManager()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mDevice);
	SAFE_RELEASE(mContext);
	if(mContext)
		mContext->ClearState();
	
	SAFE_RELEASE(mSwapChain);
}

bool CDeviceManager::init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode)
{
	mhWnd = hWnd;
	mResolution.width = width;
	mResolution.height = height;

	// D3D11_CREATE_DEVICE_FLAG 열거형에서 D3D11_CREATE_DEVICE_BGRA_SUPPORT 플래그를 사용
	unsigned int createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	// D3D_FEATURE_LEVEL 열거형에서 D3D_FEATURE_LEVEL_11_0 플래그를 사용하여 Direct3D 11.0 기능 레벨을 지정
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
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
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, &featureLevel, 1, D3D11_SDK_VERSION, &mDevice, &levelResult, &mContext)))
		return false;

	// 생성된 Device 객체의 MSAA 품질 수준을 확인하고 결과를 저장
	// DX에서는 0-1사이의 normal화 된 색상값을 사용
	// sampleCount : 1(사용하지	않음) 또는 4(4배 멀티샘플링)로 설정하여 MSAA 품질 수준을 확인
	// checkColor : 해당 샘플 개수에서 MSAA 품질 수준이 지원되는지 여부를 저장하는 변수
	unsigned int sampleCount = 4;
	unsigned int checkColor = 0;
	mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &checkColor);

	if (checkColor < 1)
		sampleCount = 1;

	/* DXGI_SWAP_CHAIN_DESC 구조체를 초기화 */
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	
	// Backbuffer 설정
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 화면주사율 설정 (1초마다 60회 화면 갱신)
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	// 화면	스케일링 설정 (Default 설정)
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// 스캔라인 순서 설정 (Default 설정)
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// 백버퍼 수 설정 (1로 설정하여 단일 백버퍼 사용)
	swapChainDesc.BufferCount = 1;
	// 백버퍼 사용 용도 설정 (렌더 타겟 출력으로 설정)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 출력 창 핸들 설정
	swapChainDesc.OutputWindow = mhWnd;
	// Multi-sampling 설정 (sampleCount 값에 따라 4배 멀티샘플링 또는 사용하지 않음)
	// DX지원 Multisampling 속도가 낮으므로 NVIDIA FSAA 기능을 활용하는 것을 권장
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = sampleCount;
	// 창 모드 설정
	swapChainDesc.Windowed = windowMode;
	// 스왑 효과 설정 (DXGI_SWAP_EFFECT_DISCARD로 설정하여 이전 프레임 버퍼 내용이 유지되지 않도록 함)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	/* 스왑 체인 생성 */
	// IUnknown 인터페이스를 상속받는 객체들은 AddRef와 Release 메서드를 통해 참조 카운트를 관리

	// type에 맞는 iid를 얻어오는 __uuidof 연산자를 사용하여 
	// mDevice 객체에서 IDXGIDevice 인터페이스 포인터를 얻어와 dxgiDevice 변수에 저장
	IDXGIDevice* dxgiDevice = nullptr;
	mDevice->QueryInterface(__uuidof(IDXGIDevice),(void**)&dxgiDevice);

	// IDXGIDevice 인터페이스 포인터를 사용하여 IDXGIAdapter 인터페이스 포인터를 얻어와 dxgiAdapter 변수에 저장
	IDXGIAdapter* dxgiAdapter = nullptr;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	// IDXGIAdapter 인터페이스 포인터를 사용하여 IDXGIFactory 인터페이스 포인터를 얻어와 dxgiFactory 변수에 저장
	IDXGIFactory* dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	// IDXGIFactory 인터페이스 포인터의 CreateSwapChain 메서드를 호출하여 스왑 체인 생성
	if (FAILED(dxgiFactory->CreateSwapChain(mDevice, &swapChainDesc, &mSwapChain)))
	{
		SAFE_RELEASE(dxgiFactory);
		SAFE_RELEASE(dxgiAdapter);
		SAFE_RELEASE(dxgiDevice);
		return false;
	}

	// 사용 종료 시 각 인터페이스 포인터의 Release 메서드를 호출하여 참조 카운트 제어
	SAFE_RELEASE(dxgiFactory);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiDevice);

	/* 렌더 타겟 뷰 생성 */ 
	// swapChain으로 생성한 BackBuffer를 렌더 타겟 뷰로 사용하기 위해 ID3D11Texture2D 인터페이스 포인터에 저장
	ID3D11Texture2D* backBuffer = nullptr;

	// Getbuffer 사용 시 AddRef가 호출되어 참조 카운트가 증가
	if(FAILED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
	{
		SAFE_RELEASE(backBuffer);
		return false;
	}

	// backBuffer에 출력하기 위한 렌더 타겟 뷰 생성
	if (FAILED(mDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView)))
	{
		SAFE_RELEASE(backBuffer);
		return false;
	}

	/* DepthStencilView Buffer로 사용할 텍스처 설정 D3D11_TEXTURE2D_DESC 구조체 초기화 */
	D3D11_TEXTURE2D_DESC depthDesc = {};

	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 그래픽카드에 해당 리소스가 DepthStencilView로 사용될 것임을 알리기 위해 D3D11_BIND_DEPTH_STENCIL 플래그 설정
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	// GPU에서 해당 리소스를 읽고쓰기를 모두 수행한다는	것을 알리기 위해 D3D11_USAGE_DEFAULT로 설정
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.SampleDesc.Count = sampleCount;
	// Default 설정
	depthDesc.SampleDesc.Quality = 0;
	// MipLevels는 텍스처의 MIP 맵 레벨 수를 지정하는 멤버
	// Depth Buffer는 화면과 1:1로 대응되어 축소된 이미지가 필요하지 않으므로 MipLevels를 1로 설정하여 단일 레벨의 텍스처를 사용하도록 함
	depthDesc.MipLevels = 1;

	/* DepthStencil Buffer로 사용할 텍스처 생성 */
	ID3D11Texture2D* depthStencilBuffer = nullptr;

	if (FAILED(mDevice->CreateTexture2D(&depthDesc, nullptr, &depthStencilBuffer)))
	{
		SAFE_RELEASE(depthStencilBuffer);
		return false;
	}

	/* DepthStencilView 생성 */
	if (FAILED(mDevice->CreateDepthStencilView(depthStencilBuffer, nullptr, &mDepthStencilView)))
	{
		SAFE_RELEASE(depthStencilBuffer);
		return false;
	}

	/* Viewport 생성 */
	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// 렌더링할 영역을 설정하기 위해 생성한 Viewport를 장치 컨텍스트에 설정
	mContext->RSSetViewports(1, &viewport);

	SAFE_RELEASE(depthStencilBuffer);
	SAFE_RELEASE(backBuffer);
	
	return true;
}
