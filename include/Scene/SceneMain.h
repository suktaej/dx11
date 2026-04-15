#pragma once
#include "Scene.h"

class CSceneMain : public CScene
{
public:
	CSceneMain(SceneKey key);
	~CSceneMain();

private:
	bool create();

public:
	bool init() override;
};
