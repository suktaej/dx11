#include "MainCamera.h"
#include "../Component/CameraComponent.h"

CMainCamera::CMainCamera(CameraKey key)
{
}

CMainCamera::~CMainCamera()
{
}

bool CMainCamera::init()
{ 
	return true;
}

void CMainCamera::update(float dt)
{
	if (mViewTarget)
	{
		if (!mViewTarget->isActive())
			mViewTarget = nullptr;
	}
}

const XMFLOAT4X4& CMainCamera::getViewMat() const
{
	return	mViewTarget->getViewMat();
}

const XMFLOAT4X4& CMainCamera::getProjMat() const
{
	return mViewTarget->getProjMat();
}
