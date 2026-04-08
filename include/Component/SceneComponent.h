#pragma once
#include "Component.h"
class CSceneComponent :
    public CComponent
{
public:
    CSceneComponent(ComponentKey key);
    CSceneComponent(ComponentKey key, const CSceneComponent& other);
    CSceneComponent(ComponentKey key, CSceneComponent&& other) noexcept;
	~CSceneComponent() override;

protected:
	CSceneComponent* mParent = nullptr;
	std::vector<CSceneComponent*> mChildList;

	DirectX::XMFLOAT3 mRelativeScale;
	DirectX::XMFLOAT3 mRelativeRotation;
	DirectX::XMFLOAT3 mRelativePosition;
	DirectX::XMFLOAT4X4 mWorldMatrix;
	DirectX::XMFLOAT4X4 mLocalMatrix;

	bool mIsLocalDirty = true;
	bool mIsWorldDirty = true;

private:
	void setRelativeScale(const DirectX::XMFLOAT3& scale);
	void setRelativeRotation(const DirectX::XMFLOAT3& rotation);
	void setRelativePosition(const DirectX::XMFLOAT3& position);
	void invalidateTransform();
	void updateWorldTransform();
	void setWorldScale(const DirectX::XMFLOAT3& scale);
	void setWorldRotation(const DirectX::XMFLOAT3& rotation);
	void setWorldPosition(const DirectX::XMFLOAT3& position);

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
	void eraseOwner() override;
	void addChild(CSceneComponent& child);

public:
	const DirectX::XMFLOAT3& getRelativeScale() const { return mRelativeScale; }
	const DirectX::XMFLOAT3& getRelativePosition() const { return mRelativePosition; }
	const DirectX::XMFLOAT3& getRelativeRotation() const { return mRelativeRotation; }
	const DirectX::XMFLOAT4X4& getWorldMatrix();
	//DirectX::XMFLOAT3 getWorldScale() { return mWorldMatrix; }
	//DirectX::XMFLOAT3 getWorldPosition() { return mWorldPosition; }
	//DirectX::XMFLOAT3 getWorldRotation() { return mWorldRotation; }
};

