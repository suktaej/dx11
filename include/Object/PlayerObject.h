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
    void MoveUp(float dt);
    void MoveDown(float dt);
    void RotY(float dt);
    void RotZ(float dt);

public:
    bool init(class CScene* scene) override;
};

