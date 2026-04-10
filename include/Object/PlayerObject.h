#pragma once
#include "Object.h"

class CPlayerObject :
    public CObject
{
public:
    CPlayerObject(ObjectKey key);
    CPlayerObject(ObjectKey key, CPlayerObject& other);
    virtual ~CPlayerObject();

public:
    bool init() override;
};

