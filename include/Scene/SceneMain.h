#pragma once
#include "Scene.h"

class CSceneMain : public CScene
{
private:
	CSceneMain();
	~CSceneMain();

public:
	bool init() override;
};

