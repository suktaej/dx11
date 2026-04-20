#include "CameraManager.h"
#include "../Component/CameraComponent.h"

CCameraManager::CCameraManager(CameraKey key)
{
}

CCameraManager::~CCameraManager()
{
}

bool CCameraManager::init()
{ 
	return true;
}

void CCameraManager::update(float dt)
{
	if (mViewTarget)
	{
		if (!mViewTarget->isActive())
			mViewTarget = nullptr;
	}
}

const XMFLOAT4X4& CCameraManager::getViewMat() const
{
	return	mViewTarget->getViewMat();
}

const XMFLOAT4X4& CCameraManager::getProjMat() const
{
	return mViewTarget->getProjMat();
}
