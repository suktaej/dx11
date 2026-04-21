#pragma once
#include "Object.h"

class CCollisionObject
    : public CObject
{
public:
    CCollisionObject(ObjectKey key);
    CCollisionObject(ObjectKey key, CCollisionObject& other);
    virtual ~CCollisionObject();

private:
    class CStaticMeshComponent* mMeshComp;
    class CMovementComponent* mMove;

public:
    bool init(class CScene* scene) override;
    void update(float dt) override;
    class CMovementComponent* getMove() { return mMove; }
};
