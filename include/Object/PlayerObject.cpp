#include "PlayerObject.h"
#include "../Component/StaticMeshComponent.h"

CPlayerObject::CPlayerObject(ObjectKey key) : CObject(key)
{
}

CPlayerObject::CPlayerObject(ObjectKey key, CPlayerObject& other) : CObject(key,other)
{
}

CPlayerObject::~CPlayerObject()
{
}

bool CPlayerObject::init()
{
	CStaticMeshComponent* root = createComponent<CStaticMeshComponent>("PlayerComponent");
	root->setMesh("ColoredBox");
	root->setShader("TestShader");
	root->setWorldPosition(0.f, 0.f, 5.5f);

	setRootComponent(root);

	return true;
}
