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
    void createMesh();
    void keyBind();

    void MoveUp(float dt);
    void MoveDown(float dt);
    void RotY(float dt);
    void RotZ(float dt);
    void RotNorm(float dt);
    void RotInv(float dt);

public:
    bool init(class CScene* scene) override;
};

