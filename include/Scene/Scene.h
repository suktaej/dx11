#pragma once
#include "../GameInfo.h"

class CScene abstract
{
public:
	class SceneKey
	{
		friend class CSceneManager;	
		SceneKey() {}
	};

	CScene(SceneKey key);
	virtual ~CScene();

protected:
	std::vector<std::unique_ptr<class CObject>> mObjectList;

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
	void processObject(float dt, std::function<void(class CObject*, float)> func);

public:
	template<typename T>
	T* createObject(const std::string& name)
	{
		static_assert(std::is_base_of_v<CObject, T>, "T must be derived from CObject");

		//std::unique_ptr<T> newObj(T(typename T::ObjectKey{}));
		std::unique_ptr<T> newObj = std::make_unique<T>(typename T::ObjectKey{});
		newObj->setName(name);
		newObj->setScene(*this);

		if(!newObj->init())
			return nullptr;

		T* newObjPtr = newObj.get();

		mObjectList.push_back(std::move(newObj));

		return newObjPtr;
	}
};

