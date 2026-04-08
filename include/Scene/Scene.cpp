#include "Scene.h"
#include "../Object/Object.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

bool CScene::init()
{
	return true;
}

bool CScene::init(const char* filePath)
{
	return true;
}

void CScene::prevUpdate(float dt)
{
	auto it = mObjectList.begin();

	while (it != mObjectList.end())
	{
		if (!(*it)->isActive())
		{
			it = mObjectList.erase(it);
			continue;
		}

		if (!(*it)->isEnabled())
		{
			++it;
			continue;
		}

		(*it)->prevUpdate(dt);
		++it;
	}
}

void CScene::update(float dt)
{
	auto it = mObjectList.begin();

	while (it != mObjectList.end())
	{
		if (!(*it)->isActive())
		{
			it = mObjectList.erase(it);
			continue;
		}

		if (!(*it)->isEnabled())
		{
			++it;
			continue;
		}

		(*it)->update(dt);
		++it;
	}
}

void CScene::postUpdate(float dt)
{
	auto it = mObjectList.begin();

	while (it != mObjectList.end())
	{
		if (!(*it)->isActive())
		{
			it = mObjectList.erase(it);
			continue;
		}

		if (!(*it)->isEnabled())
		{
			++it;
			continue;
		}

		(*it)->postUpdate(dt);
		++it;
	}
}

void CScene::prevCollision(float dt)
{
}

void CScene::collision(float dt)
{
}

void CScene::postCollision(float dt)
{
}

void CScene::prevRender()
{
	auto it = mObjectList.begin();

	while (it != mObjectList.end())
	{
		if (!(*it)->isActive())
		{
			it = mObjectList.erase(it);
			continue;
		}

		if (!(*it)->isEnabled())
		{
			++it;
			continue;
		}

		(*it)->prevRender();
		++it;
	}
}

void CScene::render()
{
	auto it = mObjectList.begin();

	while (it != mObjectList.end())
	{
		if (!(*it)->isActive())
		{
			it = mObjectList.erase(it);
			continue;
		}

		if (!(*it)->isEnabled())
		{
			++it;
			continue;
		}

		(*it)->render();
		++it;
	}
}

void CScene::postRender()
{
	auto it = mObjectList.begin();

	while (it != mObjectList.end())
	{
		if (!(*it)->isActive())
		{
			it = mObjectList.erase(it);
			continue;
		}

		if (!(*it)->isEnabled())
		{
			++it;
			continue;
		}

		(*it)->postRender();
		++it;
	}
}
