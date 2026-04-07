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
}

void CScene::update(float dt)
{
}

void CScene::postUpdate(float dt)
{
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
}

void CScene::render()
{
}

void CScene::postRender()
{
}
