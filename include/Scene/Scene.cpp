#include "Scene.h"
#include "../Object/Object.h"
#include "InputContext.h"

CScene::CScene(SceneKey key)
{
}

CScene::~CScene()
{
}

bool CScene::init()
{
	mInput = std::make_unique<CInputContext>(CInputContext::InputKey{});
	if (!mInput->init()) return false;

	return true;
}

bool CScene::init(const char* filePath)
{
	mInput = std::make_unique<CInputContext>(CInputContext::InputKey{});
	if (!mInput->init()) return false;
	
	return true;
}

void CScene::input(float dt)
{
	mInput->update(dt);
}

void CScene::prevUpdate(float dt)
{
	processObject([dt](CObject* obj)
		{ obj->prevUpdate(dt); });
}

void CScene::update(float dt)
{
	processObject([dt](CObject* obj)
		{ obj->update(dt); });
}

void CScene::postUpdate(float dt)
{
	processObject([dt](CObject* obj)
		{ obj->postUpdate(dt); });

	objectCleanUp();
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

	processObject([](CObject* obj)
		{ obj->prevRender(); });
}

void CScene::render()
{
	float dt = 0.f;

	processObject([](CObject* obj)
		{ obj->render(); });
}

void CScene::postRender()
{
	float dt = 0.f;

	processObject([](CObject* obj)
		{ obj->postRender(); });
}

void CScene::objectCleanUp()
{
	auto it = mObjectList.begin();

	while (it != mObjectList.end())
	{
		if (!(*it)->isActive())
		{
			it = mObjectList.erase(it);
			continue;
		}
		else if (!(*it)->isEnabled())
		{
			++it;
			continue;
		}

		(*it)->componentCleanUp();
		++it;
	}
}
