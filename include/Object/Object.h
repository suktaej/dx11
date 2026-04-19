#pragma once
#include "../GameInfo.h"

class CObject abstract
{
	//friend struct std::default_delete<CObject>;
public:
	class ObjectKey
	{
		friend class CScene;
		ObjectKey() {}
	};

public:
	// passkey 패턴을 사용하여 호출 클래스에서만 해당 객체를 생성할 수 있도록 함
	// passkey가 없으면 컴파일 에러가 발생하므로, 외부에서 CObject를 직접 생성할 수 없음
	// 외부에서 생성할 수 없으므로 소멸자 역시 public으로 선언
	// 단, unique_ptr로 관리 시, 엄격한 소멸자 제어를 하려면 
	// 소멸자도 private으로 선언하고 std::default_delete를 friend로 지정
	CObject(ObjectKey key);
	CObject(ObjectKey key,const CObject& other);
	CObject(ObjectKey key,CObject&& other) noexcept;
	virtual ~CObject();

protected:
	class CScene* mScene = nullptr;
	class CComponent* mRootComponent = nullptr;
	std::vector<std::unique_ptr<class CComponent>> mNonSceneCompList;
	std::vector<std::unique_ptr<class CComponent>> mSceneCompList;
	std::string mName;
	bool mIsActive = true;
	bool mIsEnabled = true;
	bool mIsVisible = true;
	float mLifeTime = 0.f;
	float mSpeed = 0.f;

public:
	virtual bool init(class CScene* scene);
	virtual bool init(class CScene* scene, const char* filePath);
	virtual void prevUpdate(float dt);
	virtual void update(float dt);
	virtual void postUpdate(float dt);
	//virtual void prevCollision(float dt);
	virtual void collision(float dt);
	//virtual void postCollision(float dt);
	virtual void prevRender();
	virtual void render();
	virtual void postRender();
	void destroy();
	
	void componentCleanUp();
	void lifeTimer(float dt);

private:
	void sceneCompCleanUp();
	void nonSceneCompCleanUp();
	void nonSceneCompUpdate(float dt, std::function<void(class CComponent*, float)> func);

public:
	class CScene* getScene() const { return mScene; }
	void setScene(class CScene& scene) { mScene = &scene; }
	void setName(const std::string& name) { mName = name; }
	const char* getName() const { return mName.c_str(); }
	void setEnabled(bool enabled) { mIsEnabled = enabled; }
	bool isEnabled() const { return mIsEnabled; }
	void setActive(bool active) { mIsActive = active; }
	bool isActive() const { return mIsActive; }
	void setLifeTime(float time) { mLifeTime = time; }
	float getLifeTime() { return mLifeTime; }
	void setSpeed(float speed) { mSpeed = speed; }
	float getSpeed() { return mSpeed; }
	void setRootComponent(class CComponent* root) { mRootComponent = root; }
	class CComponent* getRootComponent() { return mRootComponent; }

public:
	template<typename T>
	T* createComponent(const std::string& name = "Component")
	{
		static_assert(std::is_base_of_v<CComponent, T>, "T must inherit from CComponent");

		std::unique_ptr<T> newComp = std::make_unique<T>(typename T::ComponentKey{});

		if (!newComp->init())
			return nullptr;

		newComp->setName(name);
		
		T* newCompPtr = newComp.get();

		class CSceneComponent* sceneComp = dynamic_cast<class CSceneComponent*>(newCompPtr);

		if (sceneComp)
			mSceneCompList.emplace_back(std::move(newComp));
		else
			mNonSceneCompList.emplace_back(std::move(newComp));

		return newCompPtr;
	}
};

