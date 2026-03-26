#pragma once
#include "GameInfo.h"

class CDeviceManager
{
	//DECLARE_SINGLE(CDeviceManager);
public:
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
	ComPtr<ID3D11RenderTargetView> mMSAARenderTargetView[3]; 
	// MSAA 텍스처를 위한 SRV 배열 (Color, Normal, Specular)
	ComPtr<ID3D11ShaderResourceView> mMSAAShaderResourceView[3];
	// 깊이-스텐실 뷰 포인터를 멤버 변수로 선언.
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	
	FResolution mResolution;
	HWND mhWnd = nullptr;

public:
	bool init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode);
	void clearRenderTarget(const FLOAT clearColor[4]);
	void clearDepthStencilView(float depthClearValue = 1.0f, UINT8 stencilClearValue = 0);
	void setTarget();
};

