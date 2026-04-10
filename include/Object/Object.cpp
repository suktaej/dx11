#include "Object.h"
#include "../Component/Component.h"

CObject::CObject(ObjectKey key)
{
}

CObject::CObject(ObjectKey key, const CObject& other)
{
}

CObject::CObject(ObjectKey key, CObject&& other) noexcept
{
}

CObject::~CObject()
{
}

bool CObject::init()
{
	return true;
}

bool CObject::init(const char* filePath)
{
	return true;
}

void CObject::prevUpdate(float dt)
{
	mRootComponent->preUpdate(dt);
}

void CObject::update(float dt)
{
	mRootComponent->update(dt);
}

void CObject::postUpdate(float dt)
{
	mRootComponent->postUpdate(dt);
}

void CObject::collision(float dt)
{
	mRootComponent->collision(dt);
}

void CObject::prevRender()
{
	mRootComponent->preRender();
}

void CObject::render()
{
	mRootComponent->render();
}

void CObject::postRender()
{
	mRootComponent->postRender();
}
