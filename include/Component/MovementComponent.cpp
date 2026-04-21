#include "MovementComponent.h"
#include "SceneComponent.h"

CMovementComponent::CMovementComponent(ComponentKey key) 
    : CComponent(key)
{
}

CMovementComponent::CMovementComponent(ComponentKey key, const CComponent& other)
    :CComponent(key, other)
{
}

CMovementComponent::~CMovementComponent()
{
}

void CMovementComponent::addDirection(float x, float y, float z, ENegative flag)
{
    switch (flag)
    {
    case ENegative::None:
    {
        mDirection.x += x;
        mDirection.y += y;
        mDirection.z += z;
        break;
    }
    case ENegative::Negative:
    {
        mDirection.x -= x;
        mDirection.y -= y;
        mDirection.z -= z;
        break;
    }
    }
}

void CMovementComponent::addDirection(const DirectX::XMFLOAT3& dir, ENegative flag)
{
    switch (flag)
    {
    case ENegative::None:
    {
        mDirection.x += dir.x;
        mDirection.y += dir.y;
        mDirection.z += dir.z;
        break;
    }
    case ENegative::Negative:
    {
        mDirection.x -= dir.x;
        mDirection.y -= dir.y;
        mDirection.z -= dir.z;
        break;
    }
    }
}

void CMovementComponent::addDirection(const DirectX::XMVECTOR& dir, ENegative flag)
{
    DirectX::XMFLOAT3 temp;
    DirectX::XMStoreFloat3(&temp, dir); 

    switch (flag)
    {
    case ENegative::None:
    {
        mDirection.x += temp.x;
        mDirection.y += temp.y;
        mDirection.z += temp.z;
        break;
    }
    case ENegative::Negative:
    {
        mDirection.x -= temp.x;
        mDirection.y -= temp.y;
        mDirection.z -= temp.z;
        break;
    }
    }
}

const float CMovementComponent::getDistance() const
{
    DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&mMoveAmount);
    DirectX::XMVECTOR len= DirectX::XMVector3Length(vec);
    return DirectX::XMVectorGetX(len);
}

bool CMovementComponent::init()
{
    if(!CComponent::init())
		return false;

    return true;
}

bool CMovementComponent::init(const char* name)
{
    if (!CComponent::init(name))
        return false;

    return true;
}

void CMovementComponent::preUpdate(float dt)
{
    CComponent::preUpdate(dt);

    if (mUpdateComponent)
    {
        if (!mUpdateComponent->isActive())
            mUpdateComponent = nullptr;
    }
}

void CMovementComponent::update(float dt)
{
    CComponent::update(dt);

    if (mUpdateComponent)
    {
        if (!mUpdateComponent->isActive())
            mUpdateComponent = nullptr;
        else
        {
            using namespace DirectX;

            XMVECTOR vel = XMLoadFloat3(&mDirection);
            XMVECTOR unit = XMVector3Normalize(vel);
            XMVECTOR len = XMVector3Length(vel);

            if (XMVectorGetX(len) > 0.f)
            {
                float dist = mSpeed * dt;
                XMVECTOR amount = XMVectorScale(unit,dist);
                XMStoreFloat3(&mMoveAmount, amount);

                mUpdateComponent->addWorldPosition(mMoveAmount);
            }
        }
    }
}

void CMovementComponent::postUpdate(float dt)
{
    CComponent::postUpdate(dt);
}

void CMovementComponent::collision(float dt)
{
    CComponent::collision(dt);
}

void CMovementComponent::preRender()
{
    CComponent::preRender();
}

void CMovementComponent::render()
{
    CComponent::render();
}

void CMovementComponent::postRender()
{
    CComponent::postRender();

    if(mVelocity)
		mDirection = { 0.f,0.f,0.f };
}

std::unique_ptr<CComponent> CMovementComponent::clone() const
{
    return std::make_unique<CMovementComponent>(createKey(), *this);
}
