#include "CameraObject.h"
#include "../Scene/Scene.h"
#include "../Scene/InputContext.h"
#include "../Component/CameraComponent.h"
#include "../Component/MovementComponent.h"
#include "../ServiceLocator.h"

CCameraObject::CCameraObject(ObjectKey key) : CObject(key)
{
}

CCameraObject::~CCameraObject()
{
}

bool CCameraObject::init(class CScene* scene)
{
	CObject::init(scene);

	mCam = createComponent<CCameraComponent>("CameraComponent");
	setRootComponent(mCam);

	mMove = createComponent<CMovementComponent>("CameraMovemnent");
	mMove->setUpdateComponent(mCam);
	mMove->setSpeed(50.f);

	inputBind();
	
	return true;
}

void CCameraObject::update(float dt)
{
	CObject::update(dt);
}

void CCameraObject::inputBind()
{
	mInput = mScene->getInput();

	mInput->addBindKey("Forward", 'W');
	mInput->addBindKey("Backward", 'S');
	mInput->addBindKey("Right", 'D');
	mInput->addBindKey("Left", 'A');
	mInput->addBindKey("Up", 'Q');
	mInput->addBindKey("Down", 'E');
	mInput->addBindKey("Rotation", VK_RBUTTON);

	mInput->BindAction(this, &CCameraObject::MoveForward, "Forward", EInputType::Hold);
	mInput->BindAction(this, &CCameraObject::MoveBackward, "Backward", EInputType::Hold);
	mInput->BindAction(this, &CCameraObject::MoveRight, "Right", EInputType::Hold);
	mInput->BindAction(this, &CCameraObject::MoveLeft, "Left", EInputType::Hold);
	mInput->BindAction(this, &CCameraObject::MoveUp, "Up", EInputType::Hold);
	mInput->BindAction(this, &CCameraObject::MoveDown, "Down", EInputType::Hold);
	mInput->BindAction(this, &CCameraObject::Rotation, "Rotation", EInputType::Hold);
}

void CCameraObject::MoveForward(float dt)
{
	CSceneComponent* root = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (root)
		mMove->addDirection(root->getForwardVector());
}

void CCameraObject::MoveBackward(float dt)
{
	CSceneComponent* root = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (root)
		mMove->addDirection(root->getForwardVector(),ENegative::Negative);
}

void CCameraObject::MoveRight(float dt)
{
	CSceneComponent* root = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (root)
		mMove->addDirection(root->getRightVector());
}

void CCameraObject::MoveLeft(float dt)
{
	CSceneComponent* root = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (root)
		mMove->addDirection(root->getRightVector(), ENegative::Negative);
}

void CCameraObject::MoveUp(float dt)
{
	CSceneComponent* root = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (root)
		mMove->addDirection(root->getUpVector());
}

void CCameraObject::MoveDown(float dt)
{
	CSceneComponent* root = dynamic_cast<CSceneComponent*>(mRootComponent);

	if (root)
		mMove->addDirection(root->getUpVector(), ENegative::Negative);
}

void CCameraObject::Rotation(float dt)
{
	constexpr float sensitivity = 0.1f;

	IInput& input = CServiceLocator::getInput();
	XMFLOAT2 amount = input.getMouseMoveAmount();

	mYaw += amount.x * sensitivity;
	mPitch += -amount.y * sensitivity;
	mPitch = max(-89.f, min(89.f, mPitch));

	mCam->setWorldRotation(mPitch, mYaw, 0.f);
}
