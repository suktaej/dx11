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
	DirectX::XMFLOAT3 mLocalPosition;
	DirectX::XMFLOAT4 mLocalRotation;
	//DirectX::XMFLOAT3 mEulerRoatation;
	DirectX::XMFLOAT4X4 mLocalMatrix;
	DirectX::XMFLOAT4X4 mWorldMatrix;

	bool mIsLocalDirty = true;
	bool mIsWorldDirty = true;

	enum class EAxis { x, y, z, end };
	struct FAxis {
		static DirectX::XMVECTOR Get(EAxis axis) 
		{
			switch (axis) {
			case EAxis::x: return DirectX::g_XMIdentityR0; // {1, 0, 0, 0}
			case EAxis::y: return DirectX::g_XMIdentityR1; // {0, 1, 0, 0}
			case EAxis::z: return DirectX::g_XMIdentityR2; // {0, 0, 1, 0}
			default: return DirectX::XMVectorZero();
			}
		}
	};

private:
	void updateWorldTransform();
	void invalidateTransform();
	bool isDescendant(CSceneComponent* node);
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
	void eraseOwner() override;
	void addChild(CSceneComponent& child);

public:
	void setLocalScale(const DirectX::XMFLOAT3& scale);
	void setLocalScale(const EAxis& axis, const float& scale);
	void setLocalPosition(const DirectX::XMFLOAT3& position);
	void setLocalPosition(const EAxis& axis, const float& pos);
	void setLocalRotation(const DirectX::XMFLOAT3& rotation);

	void setWorldScale(const DirectX::XMFLOAT3& scale);
	void setWorldScale(const EAxis& axis, const float& scale);
	void setWorldPosition(const DirectX::XMFLOAT3& position);
	void setWorldPosition(const EAxis& axis, const float& pos);
	void setWorldRotation(const DirectX::XMFLOAT3& rotation);
	
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
	DirectX::XMFLOAT3 getWorldScale();
	DirectX::XMFLOAT3 getWorldPosition();
	DirectX::XMFLOAT4 getWorldRotation();
	
	//void setLocalRotation(EAxis axis, const float& angle);
	//void setWorldRotation(EAxis axis, const float& rotation);
};
