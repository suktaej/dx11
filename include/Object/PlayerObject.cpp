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

	//float speed = 200.f * dt;
	//mPivot->addLocalRotation(EAxis::x, speed);
}

void CPlayerObject::createMesh()
{
	mObjRoot = createComponent<CStaticMeshComponent>("PlayerComponent");
	mObjRoot->setMesh("ColoredBox");
	mObjRoot->setShader("ColorMeshShader");
	mObjRoot->setWorldScale(2.f, 2.f, 2.f);

	setRootComponent(mObjRoot);

	mPivot = createComponent<CSceneComponent>("Pivot");
	mObjRoot->addChild(*mPivot);

	mPivotRot = createComponent<CMovementComponent>("PivotRot");
	mPivotRot->setUpdateComponent(mPivot);
	mPivotRot->setRotVelocity(false);
	mPivotRot->setRotZ(180.f);
	
	mSub = createComponent<CStaticMeshComponent>("Obit");
	mPivot->addChild(*mSub);
	mSub->setMesh("ColoredSphere");
	mSub->setShader("ColorMeshShader");
	mSub->setLocalPosition(10.f, 0.f, 0.f);
	mSub->setLocalScale(0.5f, 0.5f, 0.5f);
	
	mCamera = createComponent<CCameraComponent>("Camera");
	mObjRoot->addChild(*mCamera);
	mCamera->setCameraRotation(20.f, 0.f, 0.f);
	mCamera->setCameraPosition(0.f, 10.f, -20.f);

	mMove = createComponent<CMovementComponent>("Move");
	mMove->setUpdateComponent(mObjRoot);
	mMove->setSpeed(50.f);
}

void CPlayerObject::keyBind()
{
	CInputContext* input = mScene->getInput();

	input->addBindKey("MoveForward", 'W');
	input->addBindKey("MoveBackward", 'S');
	input->addBindKey("MoveRight", 'D');
	input->addBindKey("MoveLeft", 'A');
	input->addBindKey("MoveUp", 'Q');
	input->addBindKey("MoveDown", 'E');

	input->addBindKey("RotX", 'Z');
	input->addBindKey("RotXInv", 'Z');
	input->setModifier("RotXInv", EModifier::Ctrl);

	input->addBindKey("RotY", 'X');
	input->addBindKey("RotYInv", 'X');
	input->setModifier("RotYInv", EModifier::Ctrl);

	input->addBindKey("RotZ", 'C');
	input->addBindKey("RotZInv", 'C');
	input->setModifier("RotZInv", EModifier::Ctrl);


	input->addBindKey("Fire", VK_SPACE);

	input->BindAction(this, &CPlayerObject::MoveForward, "MoveForward", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::MoveBackward, "MoveBackward", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::MoveRight, "MoveRight", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::MoveLeft, "MoveLeft", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::MoveDown, "MoveDown", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::MoveUp, "MoveUp", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotY, "RotY", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotZ, "RotZ", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotX, "RotX", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotXInv, "RotXInv", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotYInv, "RotYInv", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::RotZInv, "RotZInv", EInputType::Hold);
	input->BindAction(this, &CPlayerObject::Fire, "Fire", EInputType::Down);
}

void CPlayerObject::MoveForward(float dt)
{
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent); 

	if (sceneComp)
		mMove->addDirection(sceneComp->getForwardVector());
}

void CPlayerObject::MoveBackward(float dt)
{
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent); 

	if (sceneComp)
		mMove->addDirection(sceneComp->getForwardVector(),ENegative::Negative);
}

void CPlayerObject::MoveUp(float dt)
{
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
		mMove->addDirection(sceneComp->getUpVector());
}

void CPlayerObject::MoveDown(float dt)
{
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
		mMove->addDirection(sceneComp->getUpVector(), ENegative::Negative);
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
		mMove->addDirection(sceneComp->getRightVector(), ENegative::Negative);
}

void CPlayerObject::RotX(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);
	
	if (sceneComp)
		mMove->setRotX(90.f);
}

void CPlayerObject::RotY(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
		mMove->setRotY(90.f);
}

void CPlayerObject::RotZ(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
		mMove->setRotZ(90.f);
}

void CPlayerObject::RotXInv(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
		mMove->setRotX(-90.f);
}

void CPlayerObject::RotYInv(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
		mMove->setRotY(-90.f);
}

void CPlayerObject::RotZInv(float dt)
{
	using namespace DirectX;
	CSceneComponent* sceneComp = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (sceneComp)
		mMove->setRotZ(-90.f);
}

void CPlayerObject::Fire(float dt)
{
	CCollisionObject* obj = mScene->createObject<CCollisionObject>();
	obj->setLifeTime(1.f);
	CComponent* root = obj->getRootComponent();
	CSceneComponent* sceneRoot = dynamic_cast<CSceneComponent*>(root);
	CMovementComponent* move = obj->getMove();
	
	CSceneComponent* thisRoot = dynamic_cast<CSceneComponent*>(mRootComponent);
	move->setSpeed(50.f);
	move->setVelocity(false);
	move->setDirection(thisRoot->getForwardVector());

	sceneRoot->setWorldRotation(thisRoot->getWorldRotation());
	sceneRoot->setWorldPosition(thisRoot->getWorldPosition());
}
