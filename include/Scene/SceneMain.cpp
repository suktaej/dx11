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
	CCollisionObject* col = createObject<CCollisionObject>();
	CCollisionObject* col2 = createObject<CCollisionObject>();
	CCollisionObject* col3 = createObject<CCollisionObject>();
	CCollisionObject* col4 = createObject<CCollisionObject>();
	CSceneComponent* colComp;
	
	colComp	= dynamic_cast<CSceneComponent*>(col->getRootComponent());
	colComp->setLocalPosition(0.f, 10.f, 0.f);
	colComp = dynamic_cast<CSceneComponent*>(col2->getRootComponent());
	colComp->setLocalPosition(0.f, -10.f, 0.f);
	colComp = dynamic_cast<CSceneComponent*>(col3->getRootComponent());
	colComp->setLocalPosition(10.f, 0.f, 0.f);
	colComp = dynamic_cast<CSceneComponent*>(col4->getRootComponent());
	colComp->setLocalPosition(-10.f, 0.f, 0.f);

	return true;
}
