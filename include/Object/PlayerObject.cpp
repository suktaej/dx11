#include "PlayerObject.h"
#include "../Scene/Scene.h"
#include "../Scene/InputContext.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/MovementComponent.h"
#include "../Component/CameraComponent.h"
#include "CollisionObject.h"
#include "CameraObject.h"


CPlayerObject::CPlayerObject(ObjectKey key) : CObject(key)
{
}

CPlayerObject::CPlayerObject(ObjectKey key, CPlayerObject& other) : CObject(key,other)
{
}

CPlayerObject::~CPlayerObject()
{
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

	setRootComponent(mObjRoot);

	mRot = createComponent<CSceneComponent>("Rotator");
	mObjRoot->addChild(*mRot);
	
	mSub = createComponent<CStaticMeshComponent>("Obit");
	mRot->addChild(*mSub);
	mSub->setMesh("ColoredBox");
	mSub->setShader("ColorMeshShader");
	mSub->setLocalPosition(0.f, 0.f, 10.f);
	mSub->setLocalScale(0.5f, 0.5f, 0.5f);
	
	mCamera = createComponent<CCameraComponent>("Camera");
	mObjRoot->addChild(*mCamera);

	mMove = createComponent<CMovementComponent>("Move");
	mMove->setUpdateComponent(mObjRoot);
	mMove->setSpeed(5.f);
}

void CPlayerObject::keyBind()
{
	CInputContext* input = mScene->getInput();

	input->addBindKey("MoveUp", 'W');
	input->addBindKey("MoveDown", 'S');
	input->addBindKey("MoveRight", 'D');
	input->addBindKey("MoveLeft", 'A');

	input->addBindKey("RotY", 'Z');
	input->addBindKey("RotYInv", 'Z');
	input->setModifier("RotYInv", EModifier::Ctrl);

	input->addBindKey("RotZ", 'X');
	input->addBindKey("RotZInv", 'X');
	input->setModifier("RotZInv", EModifier::Ctrl);

	input->addBindKey("RotX", 'C');
	input->addBindKey("RotXInv", 'C');
	input->setModifier("RotXInv", EModifier::Ctrl);

	input->addBindKey("Fire", VK_SPACE);

	input->BindAction(this, &CPlayerObject::MoveUp, "MoveUp", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::MoveDown, "MoveDown", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::MoveRight, "MoveRight", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::MoveLeft, "MoveLeft", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotY, "RotY", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotZ, "RotZ", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotX, "RotX", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotXInv, "RotXInv", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotYInv, "RotYInv", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotZInv, "RotZInv", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::Fire, "Fire", EInputType::Down);
}

void CPlayerObject::MoveUp(float dt)
{
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent); 

	if (sceneComp)
		mMove->addDirection(sceneComp->getForwardVector());
}

void CPlayerObject::MoveDown(float dt)
{
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent); 

	if (sceneComp)
	{
		using namespace DirectX;
	
		XMFLOAT3 forward = sceneComp->getForwardVector();
		XMVECTOR vecForward = XMLoadFloat3(&forward);
		mMove->addDirection(vecForward * -1.0f);
	}
}

void CPlayerObject::MoveRight(float dt)
{
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
		mMove->addDirection(sceneComp->getRightVector());
}

void CPlayerObject::MoveLeft(float dt)
{
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
	{
		using namespace DirectX;

		XMFLOAT3 right = sceneComp->getRightVector();
		XMVECTOR vecright = XMLoadFloat3(&right);
		mMove->addDirection(vecright * -1.0f);
	}
}

void CPlayerObject::RotX(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 200.f * dt;

	if (sceneComp)
		sceneComp->addLocalRotation(EAxis::x, speed);
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

void CPlayerObject::RotXInv(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 200.f * dt;

	if (sceneComp)
		sceneComp->addLocalRotation(EAxis::x, -speed);
}

void CPlayerObject::RotYInv(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 200.f * dt;

	if (sceneComp)
		sceneComp->addLocalRotation(EAxis::y, -speed);
}

void CPlayerObject::RotZInv(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	float speed = 200.f * dt;

	if (sceneComp)
		sceneComp->addLocalRotation(EAxis::z, -speed);
}

void CPlayerObject::Fire(float dt)
{
	CCollisionObject* obj = mScene->createObject<CCollisionObject>();
	obj->setLifeTime(1.f);

	CComponent* root = obj->getRootComponent();
	CSceneComponent* sceneRoot = dynamic_cast<CSceneComponent*>(root);
	
	CSceneComponent* thisRoot = dynamic_cast<CSceneComponent*>(mRootComponent);

	sceneRoot->setWorldRotation(thisRoot->getWorldRotation());
	sceneRoot->setWorldPosition(thisRoot->getWorldPosition());
}
