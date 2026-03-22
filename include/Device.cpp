#include "Device.h"

DEFINITION_SINGLE(CDevice);

CDevice::CDevice()
{
}

CDevice::~CDevice()
{
	SAFE_RELEASE(mDevice);
	SAFE_RELEASE(mContext);
	if(mContext)
		mContext->ClearState();
	
	SAFE_RELEASE(mSwapChain);

}

bool CDevice::init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode)
{
	mhWnd = hWnd;
	mResolution.width = width;
	mResolution.height = height;

	D3D11CreateDevice;

	return false;
}
