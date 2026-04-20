#include "CameraComponent.h"
#include "../ServiceLocator.h"

CCameraComponent::CCameraComponent(ComponentKey key)
	: CSceneComponent(key)
{
}

CCameraComponent::CCameraComponent(ComponentKey key, const CCameraComponent& other)
	:CSceneComponent(key, other)
{
}

CCameraComponent::~CCameraComponent()
{
}

XMMATRIX CCameraComponent::makeViewProjMat()
{
	return XMLoadFloat4x4(&mView) * XMLoadFloat4x4(&mProjection);
}

void CCameraComponent::setProjectionType(EProjectionType type)
{
	mProjectionType = type;

	switch (mProjectionType)
	{
	case EProjectionType::Perspective:
	{
		float angle = XMConvertToRadians(mViewAngle);
		float ratio = mWidth / mHeight;
		XMMATRIX perspect = XMMatrixPerspectiveFovLH(angle, ratio, mNear, mFar);

		XMStoreFloat4x4(&mProjection, perspect);
		break;
	}
	case EProjectionType::Ortho:
	{
		XMMATRIX ortho = XMMatrixOrthographicOffCenterLH(
			mWidth / -2.f, mWidth / 2.f,
			mHeight / -2.f, mHeight / 2.f,
			mNear, mFar);

		XMStoreFloat4x4(&mProjection, ortho);
		break;
	}
	}
}

bool CCameraComponent::init()
{
	CSceneComponent::init();

	IDevice& device = CServiceLocator::getDevice();
	FResolution value = device.getResolution();
	mWidth = value.width;
	mHeight = value.height;

	return true;
}

bool CCameraComponent::init(const char* name)
{
	CSceneComponent::init(name);
	return true;
}

void CCameraComponent::preUpdate(float dt)
{
	CSceneComponent::preUpdate(dt);
}

void CCameraComponent::update(float dt)
{
	CSceneComponent::update(dt);
}

void CCameraComponent::postUpdate(float dt)
{
	CSceneComponent::postUpdate(dt);
}

void CCameraComponent::collision(float dt)
{
	CSceneComponent::collision(dt);
}

void CCameraComponent::preRender()
{
	CSceneComponent::preRender();
}

void CCameraComponent::render()
{
	CSceneComponent::render();
}

void CCameraComponent::postRender()
{
	CSceneComponent::postRender();

	makeViewProjMat();
}

std::unique_ptr<CComponent> CCameraComponent::clone() const
{
	return std::make_unique<CCameraComponent>(createKey(),*this);
}
