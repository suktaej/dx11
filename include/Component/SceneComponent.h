#pragma once
#include "Component.h"
class CSceneComponent :
	public CComponent
{
public:
	CSceneComponent(ComponentKey key);
	CSceneComponent(ComponentKey key, const CSceneComponent& other);
	//CSceneComponent(ComponentKey key, CSceneComponent&& other) noexcept;
	~CSceneComponent() override;

protected:
	CSceneComponent* mParent = nullptr;
	std::vector<CSceneComponent*> mChildList;

	DirectX::XMFLOAT3 mLocalScale;
	DirectX::XMFLOAT4 mLocalRotation;
	DirectX::XMFLOAT3 mLocalPosition;
	DirectX::XMFLOAT4X4 mLocalMatrix;

	DirectX::XMFLOAT3 mWorldScale;
	DirectX::XMFLOAT4 mWorldRotation;
	DirectX::XMFLOAT3 mWorldPosition;
	DirectX::XMFLOAT4X4 mWorldMatrix;
	
	//DirectX::XMFLOAT3 mEulerRoatation;

	bool mIsLocalDirty = true;
	bool mIsWorldDirty = true;

private:
	void updateWorldTransform();
	void invalidateTransform();
	bool isDescendant(CSceneComponent* node);
	void processChildren(float dt, std::function<void(CSceneComponent*, float)> func);
	//void detachFromParent();
	//DirectX::XMFLOAT3 quaternionToEuler(DirectX::XMVECTOR quat);

public:
	bool init() override;
	bool init(const char* name) override;
	void preUpdate(float dt) override;
	void update(float dt) override;
	void postUpdate(float dt) override;
	void collision(float dt) override;
	void preRender() override;
	void render() override;
	void postRender() override;
	std::unique_ptr<CComponent> clone() const override;
	//std::unique_ptr<CComponent> cloneHierarchy() const override;
	void addChild(CSceneComponent& child);
	void eraseOwner() override;

public:
	void setLocalScale(const DirectX::XMFLOAT3& scale);
	void setLocalScale(const EAxis& axis, const float& scale);
	void setLocalScale(float x, float y, float z);

	void setLocalPosition(const DirectX::XMFLOAT3& position);
	void setLocalPosition(const EAxis& axis, const float& pos);
	void setLocalPosition(float x, float y, float z);

	void setLocalRotation(const DirectX::XMFLOAT3& rotation);
	void setLocalRotation(float x, float y, float z);

	void setWorldScale(const DirectX::XMFLOAT3& scale);
	void setWorldScale(const EAxis& axis, const float& scale);
	void setWorldScale(float x, float y, float z);

	void setWorldPosition(const DirectX::XMFLOAT3& position);
	void setWorldPosition(float x, float y, float z);
	void setWorldPosition(const EAxis& axis, const float& pos);

	void setWorldRotation(const DirectX::XMFLOAT3& rotation);
	void setWorldRotation(float x, float y, float z);
	
	void addLocalScale(const EAxis& axis, const float& scale);
	void addLocalPosition(const EAxis& axis, const float& pos);
	void addLocalRotation(const EAxis& axis, const float& angle);

	void addWorldScale(const EAxis& axis, const float& scale);
	void addWorldPosition(const EAxis& axis, const float& pos);
	void addWorldRotation(const EAxis& axis, const float& angle);

	const DirectX::XMFLOAT3& getLocalScale() { return mLocalScale; }
	const DirectX::XMFLOAT4& getLocalRotation() { return mLocalRotation; }
	const DirectX::XMFLOAT3& getLocalPosition() { return mLocalPosition; }
	
	const DirectX::XMFLOAT4X4& getWorldMatrix();
	const DirectX::XMFLOAT3 getWorldScale();
	const DirectX::XMFLOAT3 getWorldPosition();
	const DirectX::XMFLOAT4 getWorldRotation();
	
	//void setLocalRotation(EAxis axis, const float& angle);
	//void setWorldRotation(EAxis axis, const float& rotation);
};
