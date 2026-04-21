#pragma once
#include "../GameInfo.h"

class CComponent abstract
{
public:
    class ComponentKey
    {
        friend class CObject;
		friend class CComponent;
    private:
        ComponentKey() {}
    public:
        ComponentKey(const ComponentKey& key) {}
	};

    CComponent(ComponentKey key);
    CComponent(ComponentKey key, const CComponent& other);
    //CComponent(ComponentKey key, CComponent&& other) noexcept;
	virtual ~CComponent();

protected:
	class CObject* mOwner = nullptr;
	class CScene* mScene = nullptr;
	std::string mName;
	bool mIsActive = true;
	bool mIsEnabled = true;
	bool mIsVisible = true;

protected:
	ComponentKey createKey() const { return ComponentKey(); }

public:
	virtual bool init();
	virtual bool init(const char* name);
	virtual void preUpdate(float dt);
	virtual void update(float dt);
	virtual void postUpdate(float dt);
	virtual void collision(float dt);
	virtual void preRender();
	virtual void render();
	virtual void postRender();
	// TODO : CRTR Pattern 사용 고려
	//virtual std::unique_ptr<CComponent> cloneHierarchy() const = 0;
	virtual std::unique_ptr<CComponent> clone() const = 0;
	virtual void eraseOwner();

public:
	class CObject* getOwner() const { return mOwner; }
	class CScene* getScene() const { return mScene; }
	const std::string& getName() const { return mName; }
	void setName(const std::string& name) { mName = name; }
	bool isActive() { return mIsActive; }
	bool isEnable() { return mIsEnabled; }
	void setActive(bool active) { mIsActive = active; }
	void setEnable(bool enable) { mIsEnabled = enable; }
	void setScene(class CScene* scene) { mScene = scene; }
	void setOwner(class CObject* obj) { mOwner = obj; }
};

