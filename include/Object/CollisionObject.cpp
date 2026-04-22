#include "CollisionObject.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/MovementComponent.h"

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

	mMove = createComponent<CMovementComponent>();
	mMove->setUpdateComponent(mMeshComp);
	
	return true;
}

void CCollisionObject::update(float dt)
{
	CObject::update(dt);
}

void CCollisionObject::viewTarget()
{
	using namespace DirectX;

	if (mTarget)
	{
		CSceneComponent* mine = dynamic_cast<CSceneComponent*>(mRootComponent);
		CSceneComponent* target = dynamic_cast<CSceneComponent*>(mTarget->getRootComponent());
		XMFLOAT3 minePos = mine->getWorldPosition();
		XMFLOAT3 targetPos = target->getWorldPosition();
		XMFLOAT3 viewVec = { targetPos.x - minePos.x, targetPos.y - minePos.y,targetPos.z - minePos.z };
		XMVECTOR normVec = XMVector3Normalize(XMLoadFloat3(&viewVec));

		XMVECTOR upVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX lookAtMat = XMMatrixLookToLH(XMLoadFloat3(&minePos), normVec, upVec);

		XMVECTOR rotationQuat = XMQuaternionRotationMatrix(XMMatrixTranspose(lookAtMat));
		XMFLOAT4 rot;
		XMStoreFloat4(&rot,rotationQuat);
		mine->setWorldRotation(rot);
	}
}
