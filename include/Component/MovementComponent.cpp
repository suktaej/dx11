#include "MovementComponent.h"

CMovementComponent::CMovementComponent(ComponentKey key) : CComponent(key)
{
}

CMovementComponent::CMovementComponent(ComponentKey key, const CComponent& other)
    :CComponent(key, other)
{
}

CMovementComponent::~CMovementComponent()
{
}

bool CMovementComponent::init()
{
    if(CComponent::init())
		return false;

    return true;
}

bool CMovementComponent::init(const char* name)
{
    if (CComponent::init(name))
        return false;

    return true;
}

void CMovementComponent::preUpdate(float dt)
{
    CComponent::preUpdate(dt);
}

void CMovementComponent::update(float dt)
{
    CComponent::update(dt);
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
}

std::unique_ptr<CComponent> CMovementComponent::clone() const
{
    return std::make_unique<CMovementComponent>(createKey(), *this);
}
