#include "SceneManager.h"
#include "../DeviceManager.h"
#include "SceneMain.h"

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
	delete mCurrentScene;
	mCurrentScene = nullptr;
}

bool CSceneManager::init()
{
	mCurrentScene = createScene<CSceneMain>();

	return true;
}

void CSceneManager::input(float dt)
{
	mCurrentScene->input(dt);
}

void CSceneManager::update(float dt)
{
	mCurrentScene->prevUpdate(dt);
	mCurrentScene->update(dt);
	mCurrentScene->postUpdate(dt);
}

void CSceneManager::render()
{
	mCurrentScene->prevRender();
	mCurrentScene->render();
	mCurrentScene->postRender();
}
void CSceneManager::collision(float dt)
{
}
