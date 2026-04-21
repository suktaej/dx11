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
	CComponent* obj1Comp = obj1->getRootComponent();
	CSceneComponent* obj1SceneComp = dynamic_cast<CSceneComponent*>(obj1Comp);
	obj1SceneComp->setLocalPosition(0.f, 0.f, 10.f);

	CCollisionObject* obj2 = createObject<CCollisionObject>("Obj");
	CComponent* obj2Comp = obj2->getRootComponent();
	CSceneComponent* obj2SceneComp = dynamic_cast<CSceneComponent*>(obj2Comp);
	obj2SceneComp->setLocalPosition(50.f, 0.f, 10.f);

	CCollisionObject* obj3 = createObject<CCollisionObject>("Obj");
	CComponent* obj3Comp = obj3->getRootComponent();
	CSceneComponent* obj3SceneComp = dynamic_cast<CSceneComponent*>(obj3Comp);
	obj3SceneComp->setLocalPosition(-50.f, 0.f, 10.f);

	return true;
}

