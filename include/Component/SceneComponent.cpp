#include "SceneComponent.h"

CSceneComponent::CSceneComponent(ComponentKey key) : CComponent(key)
{
}

CSceneComponent::CSceneComponent(ComponentKey key, const CSceneComponent& other) : CComponent(key, other)
{
}

CSceneComponent::CSceneComponent(ComponentKey key, CSceneComponent&& other) noexcept : CComponent(key, std::move(other))
{
}

CSceneComponent::~CSceneComponent()
{
}

void CSceneComponent::setRelativeScale(const DirectX::XMFLOAT3& scale)
{
    mRelativeScale = scale;
    mIsLocalDirty = true;
    invalidateTransform();
}

void CSceneComponent::setRelativeRotation(const DirectX::XMFLOAT3& rotation)
{
    using namespace DirectX;

    mRelativeRotation = rotation;
    mIsLocalDirty = true;

    invalidateTransform();
}

void CSceneComponent::setRelativePosition(const DirectX::XMFLOAT3& position)
{
    mRelativePosition = position;
    mIsLocalDirty = true;
    invalidateTransform();
}

void CSceneComponent::invalidateTransform()
{
    if (mIsWorldDirty)
        return;

    mIsWorldDirty = true;

    for (CSceneComponent* it : mChildList)
        it->invalidateTransform();
}

// Lazy Evaluation
const DirectX::XMFLOAT4X4& CSceneComponent::getWorldMatrix()
{
    //if (mIsWorldDirty || (mParent && mParent->mIsWorldDirty))
    if(mIsWorldDirty)
        updateWorldTransform();

    return mWorldMatrix;
}

void CSceneComponent::updateWorldTransform()
{
    using namespace DirectX;

    if (mIsLocalDirty)
	{
		XMMATRIX matScale = XMMatrixScaling(mRelativeScale.x, mRelativeScale.y, mRelativeScale.z);
        XMVECTOR quat = XMQuaternionRotationRollPitchYaw(
            XMConvertToRadians(mRelativeRotation.x),
            XMConvertToRadians(mRelativeRotation.y),
            XMConvertToRadians(mRelativeRotation.z));
		XMMATRIX matRot = XMMatrixRotationQuaternion(quat);
		XMMATRIX matPos = XMMatrixTranslation(mRelativePosition.x, mRelativePosition.y, mRelativePosition.z);

		XMStoreFloat4x4(&mLocalMatrix, matScale * matRot * matPos);
		mIsLocalDirty = false;
	}

    XMMATRIX matLocal = XMLoadFloat4x4(&mLocalMatrix);
    XMMATRIX matWorld;

    if (mParent)
        matWorld = matLocal * XMLoadFloat4x4(&mParent->getWorldMatrix());
    else
        matWorld = matLocal;

    XMStoreFloat4x4(&mWorldMatrix, matWorld);
    mIsWorldDirty = false;
}

void CSceneComponent::setWorldScale(const DirectX::XMFLOAT3& scale)
{
}

void CSceneComponent::setWorldRotation(const DirectX::XMFLOAT3& rotation)
{
}

void CSceneComponent::setWorldPosition(const DirectX::XMFLOAT3& position)
{
}

bool CSceneComponent::init()
{
    return true;
}

bool CSceneComponent::init(const char* name)
{
    return true;
}

void CSceneComponent::preUpdate(float dt)
{
    CComponent::preUpdate(dt);
}

void CSceneComponent::update(float dt)
{
    CComponent::update(dt);
}

void CSceneComponent::postUpdate(float dt)
{
    CComponent::postUpdate(dt);
}

void CSceneComponent::collision(float dt)
{
	CComponent::collision(dt);
}

void CSceneComponent::preRender()
{
    CComponent::preRender();
}

void CSceneComponent::render()
{
    CComponent::render();
}

void CSceneComponent::postRender()
{
    CComponent::postRender();
}

std::unique_ptr<CComponent> CSceneComponent::clone() const
{
    return std::make_unique<CSceneComponent>(createKey(), *this);
}

void CSceneComponent::eraseOwner()
{
}

void CSceneComponent::addChild(CSceneComponent& child)
{
    child.mParent = this;
	mChildList.emplace_back(&child);
}
