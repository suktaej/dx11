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
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mContext = nullptr;
	// page flip을 위한 IDXGISwapChain 포인터를 멤버 변수로 선언.
	IDXGISwapChain* mSwapChain = nullptr;
	// 렌더 타겟 뷰 포인터를 멤버 변수로 선언.
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	// 깊이-스텐실 뷰 포인터를 멤버 변수로 선언.
	ID3D11DepthStencilView* mDepthStencilView= nullptr;
	
	FResolution mResolution;
	HWND mhWnd = nullptr;

public:
	bool init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode);
};

