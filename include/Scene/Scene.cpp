#include "Scene.h"
#include "../Object/Object.h"

CScene::CScene(SceneKey key)
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
	processObject(dt, [](CObject* obj, float dt)
		{ obj->prevUpdate(dt); });
}

void CScene::update(float dt)
{
	processObject(dt, [](CObject* obj, float dt)
		{ obj->update(dt); });
}

void CScene::postUpdate(float dt)
{
	processObject(dt, [](CObject* obj, float dt)
		{ obj->postUpdate(dt); });
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
	float dt = 0.f;

	processObject(dt, [](CObject* obj, float dt)
		{ obj->prevRender(); });
}

void CScene::render()
{
	float dt = 0.f;

	processObject(dt, [](CObject* obj, float dt)
		{ obj->render(); });
}

void CScene::postRender()
{
	float dt = 0.f;

	processObject(dt, [](CObject* obj, float dt)
		{ obj->postRender(); });
}

void CScene::processObject(float dt, std::function<void(CObject*, float)> func)
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
		
		func((*it).get(), dt);
		++it;
	}
}
