#pragma once
#include "Scene.h"

class CSceneMain : public CScene
{
public:
	CSceneMain(SceneKey key);
	~CSceneMain();

public:
	bool init() override;
};

