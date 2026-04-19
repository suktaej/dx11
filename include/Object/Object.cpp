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
}

void CObject::update(float dt)
{
	mRootComponent->update(dt);

	LifeTimer(dt);
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
	auto sceneIt = mSceneCompList.begin();

	while (sceneIt != mSceneCompList.end())
	{
		if (!(*sceneIt)->isActive())
		{
			sceneIt = mSceneCompList.erase(sceneIt);
			continue;
		}
		else if (!(*sceneIt)->isEnable())
		{
			++sceneIt;
			continue;
		}

		++sceneIt;
	}

	auto nonSceneIt = mNonSceneCompList.begin();

	while (nonSceneIt != mNonSceneCompList.end())
	{
		if (!(*nonSceneIt)->isActive())
		{
			nonSceneIt = mNonSceneCompList.erase(nonSceneIt);
			continue;
		}
		else if (!(*nonSceneIt)->isEnable())
		{
			++nonSceneIt;
			continue;
		}

		++nonSceneIt;
	}
}

void CObject::LifeTimer(float dt)
{
	if (mLifeTime > 0.f)
	{
		mLifeTime -= dt;

		if (mLifeTime <= 0)
			destroy();
	}
}
