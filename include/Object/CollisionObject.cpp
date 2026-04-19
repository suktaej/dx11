#include "CollisionObject.h"
#include "../Component/StaticMeshComponent.h"

CCollisionObject::CCollisionObject(ObjectKey key) : CObject(key)
{
}

CCollisionObject::CCollisionObject(ObjectKey key, CCollisionObject& other) : CObject(key, other)
{
}

CCollisionObject::~CCollisionObject()
{
}

bool CCollisionObject::init(CScene* scene)
{
	mMeshComp = createComponent<CStaticMeshComponent>();
	mMeshComp->setMesh("ColoredBox");
	mMeshComp->setShader("ColorMeshShader");
	setRootComponent(mMeshComp);
	
	return true;
}

void CCollisionObject::update(float dt)
{
	CObject::update(dt);
	
	mMeshComp->addForwardVector(50.f * dt);
}
