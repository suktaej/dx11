#pragma once
#include "GameInfo.h"

class CDevice
{
	DECLARE_SINGLE(CDevice);

private:
	// IUnkonwn 인터페이스를 상속받는 ID3D11Device와 ID3D11DeviceContext 포인터를 멤버 변수로 선언.
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mContext = nullptr;

	// page flip을 위한 IDXGISwapChain 포인터를 멤버 변수로 선언.
	IDXGISwapChain* mSwapChain = nullptr;

	// 화면 해상도를 저장하는 FResolution 구조체 멤버 변수 선언.
	FResolution mResolution;

	HWND mhWnd = nullptr;

public:
	bool init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode);
};

