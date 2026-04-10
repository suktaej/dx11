#include "SceneMain.h"
#include "../Object/PlayerObject.h"
#include "../Component/StaticMeshComponent.h"

CSceneMain::CSceneMain(SceneKey key) 
	: CScene(key)
{
}

CSceneMain::~CSceneMain()
{
}

bool CSceneMain::init()
{
	create();

	return true;
}
bool CSceneMain::create()
{
	CPlayerObject* obj = createObject<CPlayerObject>("Player");

	return true;
}
