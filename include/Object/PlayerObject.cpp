#include "PlayerObject.h"
#include "../Scene/Scene.h"
#include "../Scene/InputContext.h"
#include "../Component/StaticMeshComponent.h"


CPlayerObject::CPlayerObject(ObjectKey key) : CObject(key)
{
}

CPlayerObject::CPlayerObject(ObjectKey key, CPlayerObject& other) : CObject(key,other)
{
}

CPlayerObject::~CPlayerObject()
{
	delete mRootComponent;
	mRootComponent = nullptr;
}

void CPlayerObject::MoveUp(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
	{
		XMVECTOR offset = XMVectorScale(FAxis::Get(EAxis::y), dt);

		XMVECTOR currentPos = XMLoadFloat3(&sceneComp->getLocalPosition());
		XMVECTOR newPos = XMVectorAdd(currentPos, offset);

		XMFLOAT3 res;
		XMStoreFloat3(&res, newPos);
		sceneComp->setLocalPosition(res);
	}
}

void CPlayerObject::MoveDown(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
		sceneComp->addLocalPosition(EAxis::y,-dt);
}

void CPlayerObject::RotY(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
	{
		float speed = 200.f * dt;

		XMVECTOR quat = XMLoadFloat4(&sceneComp->getLocalRotation());
		XMVECTOR delta = XMQuaternionRotationAxis(FAxis::Get(EAxis::y), XMConvertToRadians(speed));
		quat = XMQuaternionMultiply(quat, delta);

		XMFLOAT4 res;
		XMStoreFloat4(&res, quat);
		sceneComp->setLocalRotation(res);
	}
}

void CPlayerObject::RotZ(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 200.f * dt;

	if (sceneComp)
		sceneComp->addLocalRotation(EAxis::z, speed);
}

bool CPlayerObject::init(class CScene* scene)
{
	CObject::init(scene);

	CStaticMeshComponent* root = createComponent<CStaticMeshComponent>("PlayerComponent");
	root->setMesh("ColoredBox");
	root->setShader("ColorMeshShader");
	root->setWorldPosition(0.f, 0.f, 5.5f);
	root->setWorldRotation(0.f, 45.f, 45.f);
	root->setWorldScale(2.f, 2.f, 2.f);

	setRootComponent(root);
	
	CInputContext* input = mScene->getInput();
	
	input->addBindKey("MoveUp", 'W');
	input->addBindKey("MoveDown", 'S');
	input->addBindKey("RotY", 'Q');
	input->addBindKey("RotZ", 'E');

	input->BindAction(this, &CPlayerObject::MoveUp, "MoveUp", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::MoveDown, "MoveDown", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotY, "RotY", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotZ, "RotZ", EInputType::Hold);

	return true;
}
