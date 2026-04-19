#pragma once
#include "Object.h"

class CPlayerObject :
    public CObject
{
public:
    CPlayerObject(ObjectKey key);
    CPlayerObject(ObjectKey key, CPlayerObject& other);
    virtual ~CPlayerObject();

private:
    class CStaticMeshComponent* mObjRoot;
    class CStaticMeshComponent* mSub;
    class CSceneComponent* mRot;
    class CMovementComponent* mMove;

public:
    bool init(class CScene* scene) override;
    void update(float dt) override;

private:
    void createMesh();
    void keyBind();

    void MoveUp(float dt);
    void MoveDown(float dt);
    void RotX(float dt);
    void RotY(float dt);
    void RotZ(float dt);
    void RotXInv(float dt);
    void RotYInv(float dt);
    void RotZInv(float dt);
    void Fire(float dt);
};

