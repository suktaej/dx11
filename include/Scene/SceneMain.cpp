#include "SceneMain.h"
#include "../Object/PlayerObject.h"
#include "../InputManager.h"

CSceneMain::CSceneMain(SceneKey key) 
	: CScene(key)
{
}

CSceneMain::~CSceneMain()
{
}

bool CSceneMain::init()
{
	if (!CScene::init())
		return false;

	create();

	return true;
}


bool CSceneMain::create()
{
	CPlayerObject* obj = createObject<CPlayerObject>("Player");

	return true;
}
