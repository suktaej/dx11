#pragma once
#include "../GameInfo.h"
#include "Scene.h"

class CSceneManager
{
//public:
	friend class CGameManager;

private:
	CSceneManager();
	~CSceneManager();

private:
	// TODO : unique_ptr »Æ¿Œ
	class CScene* mCurrentScene = nullptr;

public:
	bool init();
	void input(float dt);
	void update(float dt);
	void collision(float dt);
	void render();

public:
	template <typename T>
	T* createScene()
	{
		static_assert(std::is_base_of<CScene, T>::value, "T must inherit from CScene");
		T* scene = new T(typename T::SceneKey{});

		if (!scene->init())
			return nullptr;

		return scene;
	}
};
