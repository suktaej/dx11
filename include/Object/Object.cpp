#include "Object.h"
#include "../Component/Component.h"

CObject::CObject(SceneKey key)
{
}

CObject::CObject(SceneKey key, const CObject& other)
{
}

CObject::CObject(SceneKey key, CObject&& other) noexcept
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
}

void CObject::update(float dt)
{
}

void CObject::postUpdate(float dt)
{
}

void CObject::prevCollision(float dt)
{
}

void CObject::collision(float dt)
{
}

void CObject::postCollision(float dt)
{
}

void CObject::prevRender()
{
}

void CObject::render()
{
}

void CObject::postRender()
{
}
