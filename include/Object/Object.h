#pragma once
#include "../GameInfo.h"

class CObject
{
public:
	class SceneKey
	{
		friend class CScene;
		// private 생성자를 통해 CScene에서만 CObject를 생성가능
		SceneKey() {}
	};

protected:
	CObject(SceneKey key);
	CObject(SceneKey key,const CObject& other);
	CObject(SceneKey key,CObject&& other) noexcept;
public:
	virtual ~CObject();

protected:
	class CScene* mScene = nullptr;
	std::string mName;

public:
	virtual bool init();
	virtual bool init(const char* filePath);
	virtual void prevUpdate(float dt);
	virtual void update(float dt);
	virtual void postUpdate(float dt);
	virtual void prevCollision(float dt);
	virtual void collision(float dt);
	virtual void postCollision(float dt);
	virtual void prevRender();
	virtual void render();
	virtual void postRender();

public:
	class CScene* getScene() const { return mScene; }
	void setName(const std::string& name) { mName = name; }
	const char* getName() const { return mName.c_str(); }
};

