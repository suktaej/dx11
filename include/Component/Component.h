#pragma once
#include "../Object/Object.h"

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
    CComponent(ComponentKey key, CComponent&& other) noexcept;
	virtual ~CComponent();

protected:
	class CObject* mOwner = nullptr;
	class CScene* mScene = nullptr;
	std::string mName;

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
	virtual std::unique_ptr<CComponent> clone() const = 0;
	virtual void eraseOwner();

public:
	class CObject* getOwner() const { return mOwner; }
	class CScene* getScene() const { return mScene; }
	const char* getName() const { return mName.c_str(); }
	void setName(const std::string& name) { mName = name; }
};

