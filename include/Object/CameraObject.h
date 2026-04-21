#pragma once
#include "Object.h"

class CCameraObject :
    public CObject
{
public:
    CCameraObject(ObjectKey key);
    ~CCameraObject();

private:
    class CInputContext* mInput;
    class CCameraComponent* mCam;
    class CMovementComponent* mMove;
    float mYaw;
    float mPitch;

public:
    bool init(class CScene* scene) override;
    void update(float dt) override;

private:
    void inputBind();
    void MoveForward(float dt);
    void MoveBackward(float dt);
    void MoveRight(float dt);
    void MoveLeft(float dt);
    void MoveUp(float dt);
    void MoveDown(float dt);
    void Rotation(float dt);
};

