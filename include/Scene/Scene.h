#pragma once
#include "../GameInfo.h"
#include "../Object/Object.h"

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
	std::vector<std::unique_ptr<CObject>> mObjectList;
	std::unique_ptr<class CInputContext> mInput;
	
private:
	void objectCleanUp();

public:
	virtual bool init();
	virtual bool init(const char* filePath);
	virtual void input(float dt);
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
	class CInputContext* getInput() { return mInput.get(); }

public:
	template<typename T>
	T* createObject(const std::string& name = "Object")
	{
		static_assert(std::is_base_of_v<CObject, T>, "T must be derived from CObject");

		//std::unique_ptr<T> newObj(T(typename T::ObjectKey{}));
		std::unique_ptr<T> newObj = std::make_unique<T>(typename T::ObjectKey{});
		newObj->setName(name);
		newObj->setScene(*this);

		if(!newObj->init(this))
			return nullptr;

		T* newObjPtr = newObj.get();

		mObjectList.push_back(std::move(newObj));

		return newObjPtr;
	}

private:
	template<typename F>
	void processObject(F&& func)
	{
		auto it = mObjectList.begin();

		while (it != mObjectList.end())
		{
			CObject* obj = (*it).get();

			if (!obj->isActive())
			{
				it = mObjectList.erase(it);
				continue;
			}

			if (!obj->isEnabled())
			{
				++it;
				continue;
			}

			func(obj);
			++it;
		}
	}
};

