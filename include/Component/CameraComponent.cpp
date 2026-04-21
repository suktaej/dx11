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

void CCameraComponent::updateViewMatrix()
{
	XMMATRIX worldMat = XMLoadFloat4x4(&getWorldMatrix());

	XMVECTOR pos = worldMat.r[3];
	XMVECTOR up = worldMat.r[1];
	XMVECTOR forward = worldMat.r[2];

	forward = XMVector3Normalize(forward);
	up = XMVector3Normalize(up);

	XMVECTOR at = XMVectorAdd(pos, forward);
	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(pos, at, up));
}

void CCameraComponent::calcVerticalFOV()
{
	float aspectRatio = mWidth / mHeight;
	float hFovRad = XMConvertToRadians(mHorizontalFOV);

	// 수평 FOV → 수직 FOV
	float vFovRad = 2.f * std::atan(std::tan(hFovRad / 2.f) / aspectRatio);

	mFieldOfView = XMConvertToDegrees(vFovRad);
}

void CCameraComponent::setProjectionType(EProjectionType type)
{
	mProjectionType = type;

	switch (mProjectionType)
	{
	case EProjectionType::Perspective:
	{
		float angle = XMConvertToRadians(mFieldOfView);
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

void CCameraComponent::setCameraPosition(float x, float y, float z)
{
	CSceneComponent::setLocalPosition(x, y, z);
	updateViewMatrix();
}

void CCameraComponent::setCameraRotation(float x, float y, float z)
{
	CSceneComponent::setLocalRotation(x, y, z);
	updateViewMatrix();
}

bool CCameraComponent::init()
{
	CSceneComponent::init();

	IDevice& device = CServiceLocator::getDevice();
	FResolution value = device.getResolution();
	mWidth = (float)value.width;
	mHeight = (float)value.height;
	
	calcVerticalFOV();
	updateViewMatrix();
	setProjectionType(EProjectionType::Perspective);

	ICamera& camera = CServiceLocator::getCamera();

	if (!camera.getVIewTarget())
		camera.setViewTarget(this);

	return true;
}

bool CCameraComponent::init(const char* name)
{
	CSceneComponent::init(name);

	IDevice& device = CServiceLocator::getDevice();
	FResolution value = device.getResolution();
	mWidth = (float)value.width;
	mHeight = (float)value.height;

	calcVerticalFOV();
	updateViewMatrix();
	setProjectionType(EProjectionType::Perspective);

	ICamera& camera = CServiceLocator::getCamera();

	if (!camera.getVIewTarget())
		camera.setViewTarget(this);

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
	
	updateViewMatrix();
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
}

std::unique_ptr<CComponent> CCameraComponent::clone() const
{
	return std::make_unique<CCameraComponent>(createKey(),*this);
}
