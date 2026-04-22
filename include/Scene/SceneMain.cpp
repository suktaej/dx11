#include "SceneMain.h"
#include "../Object/PlayerObject.h"
#include "../Object/CollisionObject.h"
#include "../Component/SceneComponent.h"
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
	CCollisionObject* col[10000];
	CSceneComponent* colComp = nullptr;
	
	for (int i = 0; i<100;++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			col[i*100+j] = createObject<CCollisionObject>();
			colComp = static_cast<CSceneComponent*>(col[i*100+j]->getRootComponent());
			colComp->setLocalPosition(j*10, i*10, 0.f);
		}
	}

	return true;
}
