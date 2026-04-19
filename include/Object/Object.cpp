#include "Object.h"
#include "../ServiceLocator.h"
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

bool CObject::init(class CScene* scene)
{
	mScene = scene;
	return true;
}

bool CObject::init(class CScene* scene,const char* filePath)
{
	mScene = scene;
	return true;
}

void CObject::prevUpdate(float dt)
{
	mRootComponent->preUpdate(dt);

	nonSceneCompUpdate(dt, [](CComponent* comp, float dt)
		{comp->preUpdate(dt); });
}

void CObject::update(float dt)
{
	mRootComponent->update(dt);

	nonSceneCompUpdate(dt, [](CComponent* comp, float dt)
		{comp->update(dt); });
	lifeTimer(dt);
}

void CObject::postUpdate(float dt)
{
	mRootComponent->postUpdate(dt);
	
	nonSceneCompUpdate(dt, [](CComponent* comp, float dt)
		{comp->postUpdate(dt); });
}

void CObject::collision(float dt)
{
	mRootComponent->collision(dt);
}

void CObject::prevRender()
{
	mRootComponent->preRender();
	
	float dt = 0.f;
	nonSceneCompUpdate(dt, [](CComponent* comp, float dt)
		{comp->preRender(); });
}

void CObject::render()
{
	mRootComponent->render();
	
	float dt = 0.f;
	nonSceneCompUpdate(dt, [](CComponent* comp, float dt)
		{comp->render(); });
}

void CObject::postRender()
{
	mRootComponent->postRender();
	
	float dt = 0.f;
	nonSceneCompUpdate(dt, [](CComponent* comp, float dt)
		{comp->postRender(); });
}

void CObject::destroy()
{
	mIsActive = false;

	//auto sit = mSceneCompList.begin();
	//while (sit != mSceneCompList.end())
	//	(*sit++)->setActive(false);

	mSceneCompList.clear();

	//auto nit = mNonSceneCompList.begin();
	//while (nit != mNonSceneCompList.end())
	//	(*nit++)->setActive(false);

	mNonSceneCompList.clear();
}

void CObject::componentCleanUp()
{
	sceneCompCleanUp();
	nonSceneCompCleanUp();
}

void CObject::sceneCompCleanUp()
{
	auto it = mSceneCompList.begin();

	while (it != mSceneCompList.end())
	{
		if (!(*it)->isActive())
		{
			it = mSceneCompList.erase(it);
			continue;
		}
		else if (!(*it)->isEnable())
		{
			++it;
			continue;
		}

		++it;
	}
}

void CObject::nonSceneCompCleanUp()
{
	auto it = mNonSceneCompList.begin();

	while (it != mNonSceneCompList.end())
	{
		if (!(*it)->isActive())
		{
			it = mNonSceneCompList.erase(it);
			continue;
		}
		else if (!(*it)->isEnable())
		{
			++it;
			continue;
		}

		++it;
	}
}

void CObject::nonSceneCompUpdate(float dt, std::function<void(class CComponent*, float)> func)
{
	auto it = mNonSceneCompList.begin();

	while (it != mNonSceneCompList.end())
	{
		if (!(*it)->isActive())
		{
			it = mNonSceneCompList.erase(it);
			continue;
		}
		else if (!(*it)->isEnable())
		{
			++it;
			continue;
		}
		
		func((*it).get(), dt);
		++it;
	}
}

void CObject::lifeTimer(float dt)
{
	if (mLifeTime > 0.f)
	{
		mLifeTime -= dt;

		if (mLifeTime <= 0)
			destroy();
	}
}
