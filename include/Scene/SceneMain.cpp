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
	int n = 50;

	CPlayerObject* obj = createObject<CPlayerObject>("Player");
	//CCollisionObject* col[10000];
	std::vector<CCollisionObject*> col(n*n);

	CSceneComponent* colComp = nullptr;
	
	for (int i = 0; i<n;++i)
	{
		for (int j = 0; j < n; ++j)
		{
			col[i*n+j] = createObject<CCollisionObject>();
			colComp = static_cast<CSceneComponent*>(col[i*n+j]->getRootComponent());
			colComp->setLocalPosition(j*10.f, i*10.f, 0.f);
		}
	}

	return true;
}
