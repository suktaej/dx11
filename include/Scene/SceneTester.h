#pragma once
#include "Scene.h"
class CSceneTester :
    public CScene
{
public:
	CSceneTester(SceneKey key);
	~CSceneTester();

private:
	bool create();

public:
	bool init() override;
};

