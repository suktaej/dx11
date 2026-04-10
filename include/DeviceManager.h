#pragma once
#include "GameInfo.h"
#include "ServiceInterface.h"

struct FPostProcessVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

class CDeviceManager : public IDevice
{
	//DECLARE_SINGLE(CDeviceManager);
	friend class CGameManager;
	friend std::default_delete<CDeviceManager>;

//public:
private:
	CDeviceManager();
	~CDeviceManager();

private:
	// IUnkonwn 인터페이스를 상속받는 ID3D11Device와 ID3D11DeviceContext 포인터를 멤버 변수로 선언.
	//ID3D11Device* mDevice = nullptr;
	ComPtr<ID3D11Device> mDevice;
	//ID3D11DeviceContext* mContext = nullptr;
	ComPtr<ID3D11DeviceContext> mContext;
	// page flip을 위한 IDXGISwapChain 포인터를 멤버 변수로 선언.
	//IDXGISwapChain* mSwapChain = nullptr;
	ComPtr<IDXGISwapChain1> mSwapChain;
	// 렌더 타겟 뷰 포인터를 멤버 변수로 선언.
	//ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ComPtr<ID3D11RenderTargetView> mRenderTargetView;

	// MSAA용 내부 텍스처
	ComPtr<ID3D11Texture2D> mMSAATexture[3];
	// MSAA 텍스처를 위한 RTV 배열 (Color, Normal, Specular)
	// 기하구조(Geometry) 정보를 저장
	ComPtr<ID3D11RenderTargetView> mMSAARenderTargetView[3]; 
	// MSAA 텍스처를 위한 SRV 배열 (Color, Normal, Specular)
	ComPtr<ID3D11ShaderResourceView> mMSAAShaderResourceView[3];

	// 깊이-스텐실 뷰 포인터를 멤버 변수로 선언.
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	ComPtr<ID3D11ShaderResourceView> mDepthSRV; 
	
	// 라이팅 결과가 담길 HDR 버퍼 (MSAA 대응)
	ComPtr<ID3D11Texture2D> mLightAccumTextureMSAA;
	ComPtr<ID3D11RenderTargetView> mLightAccumRTVMSAA;

	// Resolve된 HDR 버퍼 (단일 샘플, 포스트 프로세스 시작점)
	ComPtr<ID3D11Texture2D> mResolvedHDRTexture;
	ComPtr<ID3D11ShaderResourceView> mResolvedHDRSRV;
	
	// 포스트 프로세스용 핑퐁 버퍼 (최소 2개)
	// 효과가 여러 개일 때 (A -> B, B -> A 식으로 번갈아가며 렌더링)
	ComPtr<ID3D11Texture2D> mPostProcessTextures[2];
	ComPtr<ID3D11RenderTargetView> mPostProcessRTVs[2];
	ComPtr<ID3D11ShaderResourceView> mPostProcessSRVs[2];

	// 화면 전체를 덮는 삼각형/사각형 Vertex Buffer (포스트 프로세스 실행용)
	// 픽셀 셰이더만 돌리기 위해 필요
	ComPtr<ID3D11Buffer> mFullQuadVB;
	
	// 생성된 Device 객체의 MSAA 품질 수준을 확인하고 결과를 저장
	// DX에서는 0-1사이의 normal화 된 색상값을 사용
	// sampleCount : 1(사용하지	않음) 또는 4(4배 멀티샘플링)로 설정하여 MSAA 품질 수준을 확인
	// checkColor : 해당 샘플 개수에서 MSAA 품질 수준이 지원되는지 여부를 저장하는 변수
	unsigned int mSampleCount;
	unsigned int mCheckColor;
	// 화면 해상도를 저장하는 구조체 멤버 변수
	FResolution mResolution;
	// Windows 핸들 저장 (초기화 시 전달받음)
	HWND mhWnd = nullptr;

private:
	bool createDevice();                    // ID3D11Device 생성 및 MSAA 체크
	bool createSwapChain(bool windowMode);	// IDXGISwapChain 생성 및 BackBuffer RTV
	bool createDepthStencilView();			// DepthStencilView 생성
	bool createGeometryBuffers();			// G-Buffer 텍스처/뷰 생성
	bool createLightingBuffers();			// HDR 라이팅 버퍼 생성
	bool createPostProcessBuffers();		// Resolve/Ping-pong 버퍼 생성
	void setViewport();						// RSSetViewports
	bool createFullQuadBuffer();			// 화면 전체를 덮는 정점 버퍼 생성 (포스트 프로세스용)
	
	void unbindShaderResources(); // 픽셀 셰이더에 바인딩된 SRV 해제

//public:
	bool init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode);

	void clearBuffers(const FLOAT clearColor[4]);
	void BeginGeometryPass();
	void BeginLightingPass();
	void ResolveAndBeginPostProcess();
	void setPostProcessTarget(int index); // 포스트 프로세스 타겟 설정 (0 또는 1번 핑퐁 버퍼)
	void setPostProcessSource(ID3D11ShaderResourceView* srv); // 셰이더에게 읽을 소스 전달 (mResolvedHDRSRV 또는 핑퐁 SRV)
	void BeginFinalPass(); // 최종 백버퍼를 타겟으로 설정 (포스트 프로세스의 마지막 단계)
	void drawFullScreenQuad();
	void present(); // 스왑체인 Present 호출

	void serviceInit() override;
	ID3D11Device* getDevice() override;
	ID3D11DeviceContext* getContext() override;
	
	void testRender();

public:
	ID3D11Device* getDevice() const { return mDevice.Get(); }
	ID3D11DeviceContext* getContext() const { return mContext.Get(); }
	IDXGISwapChain1* getSwapChain() const { return mSwapChain.Get(); }
	FResolution getResolution() const { return mResolution; }
	ID3D11ShaderResourceView* getGBufferSRV(int index) const { return mMSAAShaderResourceView[index].Get(); }
	ID3D11ShaderResourceView* getResolvedHDRSRV() const { return mResolvedHDRSRV.Get(); }
	ID3D11ShaderResourceView* getPostProcessSRV(int index) const { return mPostProcessSRVs[index].Get(); }
};

