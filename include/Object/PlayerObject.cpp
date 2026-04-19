#include "PlayerObject.h"
#include "../Scene/Scene.h"
#include "../Scene/InputContext.h"
#include "../Component/StaticMeshComponent.h"
#include "CollisionObject.h"


CPlayerObject::CPlayerObject(ObjectKey key) : CObject(key)
{
}

CPlayerObject::CPlayerObject(ObjectKey key, CPlayerObject& other) : CObject(key,other)
{
}

CPlayerObject::~CPlayerObject()
{
	//delete mRootComponent;
}

bool CPlayerObject::init(class CScene* scene)
{
	CObject::init(scene);

	createMesh();
	keyBind();

	return true;
}

void CPlayerObject::update(float dt)
{
	CObject::update(dt);

	float speed = 200.f * dt;
	mRot->addLocalRotation(EAxis::x, speed);
}

void CPlayerObject::createMesh()
{
	mObjRoot = createComponent<CStaticMeshComponent>("PlayerComponent");
	mObjRoot->setMesh("ColoredBox");
	mObjRoot->setShader("ColorMeshShader");
	mObjRoot->setWorldPosition(0.f, 0.f, 5.5f);
	mObjRoot->setWorldRotation(0.f, 45.f, 45.f);
	mObjRoot->setWorldScale(2.f, 2.f, 2.f);

	setRootComponent(mObjRoot.get());

	mRot = createComponent<CSceneComponent>("Rotator");
	mObjRoot->addChild(*mRot);
	
	mSub = createComponent<CStaticMeshComponent>("Obit");
	mRot->addChild(*mSub);
	mSub->setMesh("ColoredBox");
	mSub->setShader("ColorMeshShader");
	mSub->setLocalPosition(0.f, 0.f, 10.f);
	mSub->setLocalScale(0.5f, 0.5f, 0.5f);
}

void CPlayerObject::keyBind()
{
	CInputContext* input = mScene->getInput();

	input->addBindKey("MoveUp", 'W');
	input->addBindKey("MoveDown", 'S');
	input->addBindKey("RotY", 'Q');
	input->addBindKey("RotZ", 'E');
	input->addBindKey("RotNorm", 'A');
	input->addBindKey("RotInv", 'D');
	input->addBindKey("Fire", VK_SPACE);

	input->BindAction(this, &CPlayerObject::MoveUp, "MoveUp", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::MoveDown, "MoveDown", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotY, "RotY", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotZ, "RotZ", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotNorm, "RotNorm", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotInv, "RotInv", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::Fire, "Fire", EInputType::Down);
}

void CPlayerObject::MoveUp(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 20.f * dt;
	if (sceneComp)
	{
		//sceneComp->addLocalPosition(EAxis::y, dt);
		DirectX::XMFLOAT3 up = sceneComp->getUpVector();
		sceneComp->addLocalPositionByDirection(up, speed);
	}
}

void CPlayerObject::MoveDown(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 20.f * dt;
	if (sceneComp)
	{
		//sceneComp->addLocalPosition(EAxis::y,-dt);
		DirectX::XMFLOAT3 up = sceneComp->getUpVector();
		sceneComp->addLocalPositionByDirection(up, -speed);
	}
}

void CPlayerObject::RotY(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 200.f * dt;

	if (sceneComp)
		sceneComp->addLocalRotation(EAxis::y, speed);
}

void CPlayerObject::RotZ(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 200.f * dt;

	if (sceneComp)
		sceneComp->addLocalRotation(EAxis::z, speed);
}

void CPlayerObject::RotNorm(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 200.f * dt;

	if (sceneComp)
		sceneComp->addLocalRotation(EAxis::x, speed);
}

void CPlayerObject::RotInv(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 200.f * dt;

	if (sceneComp)
		sceneComp->addLocalRotation(EAxis::x, -speed);
}

void CPlayerObject::Fire(float dt)
{
	CCollisionObject* obj = mScene->createObject<CCollisionObject>();

	CComponent* root = obj->getRootComponent();
	CSceneComponent* sceneRoot = dynamic_cast<CSceneComponent*>(root);
	
	CSceneComponent* thisRoot = dynamic_cast<CSceneComponent*>(mRootComponent);

	sceneRoot->setWorldRotation(thisRoot->getWorldRotation());
	sceneRoot->setWorldPosition(thisRoot->getWorldPosition());
}
