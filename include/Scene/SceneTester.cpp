#include "SceneTester.h"
#include "../Object/CameraObject.h"
#include "../Object/CollisionObject.h"
#include "../Component/SceneComponent.h"

CSceneTester::CSceneTester(SceneKey key) 
	: CScene(key)
{
}

CSceneTester::~CSceneTester()
{
}

bool CSceneTester::init()
{
	if (!CScene::init())
		return false;

	create();

	return true;
}

bool CSceneTester::create()
{
	CCameraObject* cam = createObject<CCameraObject>("CameraObject");

	CCollisionObject* obj1 = createObject<CCollisionObject>("Obj");
	CSceneComponent* obj1Comp = obj1->getRootComponent();
	obj1Comp->setLocalPosition(0.f, 0.f, 10.f);

	CCollisionObject* obj2 = createObject<CCollisionObject>("Obj");
	CSceneComponent* obj2Comp = obj2->getRootComponent();
	obj2Comp->setLocalPosition(50.f, 0.f, 10.f);

	CCollisionObject* obj3 = createObject<CCollisionObject>("Obj");
	CSceneComponent* obj3Comp = obj3->getRootComponent();
	obj3Comp->setLocalPosition(-50.f, 0.f, 10.f);

	CCollisionObject* obj4 = createObject<CCollisionObject>("Obj");
	CSceneComponent* obj4Comp = obj4->getRootComponent();
	obj4Comp->setLocalPosition(0.f, 50.f, 10.f);
	
	CCollisionObject* obj5 = createObject<CCollisionObject>("Obj");
	CSceneComponent* obj5Comp = obj5->getRootComponent();
	obj5Comp->setLocalPosition(0.f, -50.f, 10.f);

	return true;
}

