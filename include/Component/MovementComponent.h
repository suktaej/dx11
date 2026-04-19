#pragma once
#include "Component.h"

class CMovementComponent :
	public CComponent
{
public:
	CMovementComponent(ComponentKey key);
	CMovementComponent(ComponentKey key, const CComponent& other);
	virtual ~CMovementComponent();

protected:
	class CSceneComponent* mUpdateComponent = nullptr;
	DirectX::XMFLOAT3 mVelocity = { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 mMoveAmount = { 0.f, 0.f, 0.f };
	float mSpeed = 1.f;

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

public:
	void setVelocity(const DirectX::XMFLOAT3& velocity) { mVelocity = velocity; }
	void setVelocity(float x, float y, float z) { mVelocity = { x,y,z }; }
	void setVelocity(const EAxis& axis) { DirectX::XMStoreFloat3(&mVelocity, FAxis::Get(EAxis::x)); }
	void setSpeed(float speed) { mSpeed = speed; }
	void setUpdateComponent(class CSceneComponent* comp) { mUpdateComponent = comp; }

	void addVelocity(float x, float y, float z);
	void addVelocity(const DirectX::XMFLOAT3& velocity);
	void addVelocity(const DirectX::XMVECTOR& velocity);

	const DirectX::XMFLOAT3 getMoveAmount() const { return mMoveAmount; }
	const float getDistance() const;

	void addMoveX(const float& x) { mVelocity.x += x; }
	void addMoveY(const float& y) { mVelocity.y += y; }
	void addMoveZ(const float& z) { mVelocity.z += z; }

	void setMoveX(const float& x) { mVelocity.x = x; }
	void setMoveY(const float& y) { mVelocity.y = y; }
	void setMoveZ(const float& z) { mVelocity.z = z; }

	float getMoveX() { return mVelocity.x; }
	float getMoveY() { return mVelocity.y; }
	float getMoveZ() { return mVelocity.z; }

};

