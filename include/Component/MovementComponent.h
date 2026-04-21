#pragma once
#include "Component.h"

enum class ENegative
{
	None,
	Negative
};

class CMovementComponent :
	public CComponent
{
public:
	CMovementComponent(ComponentKey key);
	CMovementComponent(ComponentKey key, const CComponent& other);
	virtual ~CMovementComponent();

protected:
	class CSceneComponent* mUpdateComponent = nullptr;
	DirectX::XMFLOAT3 mDirection = { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 mMoveAmount = { 0.f, 0.f, 0.f };
	bool mVelocity = true;
	float mSpeed = 0.f;
	
	DirectX::XMFLOAT3 mRotation= { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 mRotAmount = { 0.f, 0.f, 0.f };
	bool mRotVelocity = true;
	float mRotSpeed = 0.f;

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

private:
	void updateDirection(float dt);
	void updateRotation(float dt);

public:
#pragma region Direction
	void setDirection(float x, float y, float z) { mDirection = { x,y,z }; }
	void setDirection(const DirectX::XMFLOAT3& dir) { mDirection = dir; }
	void setDirection(const DirectX::XMVECTOR& dir) { DirectX::XMStoreFloat3(&mDirection, dir); }
	void setDirection(const EAxis& axis) { DirectX::XMStoreFloat3(&mDirection, FAxis::Get(EAxis::x)); }
	void setSpeed(float speed) { mSpeed = speed; }
	void setUpdateComponent(class CSceneComponent* comp) { mUpdateComponent = comp; }

	void addDirection(float x, float y, float z, ENegative flag = ENegative::None);
	void addDirection(const DirectX::XMFLOAT3& dir, ENegative flag = ENegative::None);
	void addDirection(const DirectX::XMVECTOR& dir, ENegative flag = ENegative::None);

	const DirectX::XMFLOAT3 getMoveAmount() const { return mMoveAmount; }
	const float getDistance() const;
	void setVelocity(bool init) { mVelocity = init; }

	void addDirX(const float& x) { mDirection.x += x; }
	void addDirY(const float& y) { mDirection.y += y; }
	void addDirZ(const float& z) { mDirection.z += z; }

	void setDirX(const float& x) { mDirection.x = x; }
	void setDirY(const float& y) { mDirection.y = y; }
	void setDirZ(const float& z) { mDirection.z = z; }

	float getDirX() { return mDirection.x; }
	float getDirY() { return mDirection.y; }
	float getDirZ() { return mDirection.z; }
#pragma endregion Direction

#pragma region Rotation
	void setRotation(float x, float y, float z) { mRotation = { x,y,z }; }
	void setRotation(const DirectX::XMFLOAT3& dir) { mRotation = dir; }
	void setRotation(const DirectX::XMVECTOR& dir) { DirectX::XMStoreFloat3(&mRotation, dir); }
	void setRotation(const EAxis& axis) { DirectX::XMStoreFloat3(&mRotation, FAxis::Get(EAxis::x)); }
	void setRotSpeed(float speed) { mRotSpeed = speed; }

	void addRotation(float x, float y, float z, ENegative flag = ENegative::None);
	void addRotation(const DirectX::XMFLOAT3& dir, ENegative flag = ENegative::None);
	void addRotation(const DirectX::XMVECTOR& dir, ENegative flag = ENegative::None);

	const DirectX::XMFLOAT3 getRotAmount() const { return mRotAmount; }
	void setRotVelocity(bool init) { mRotVelocity = init; }

	void addRotX(const float& x) { mRotation.x += x; }
	void addRotY(const float& y) { mRotation.y += y; }
	void addRotZ(const float& z) { mRotation.z += z; }

	void setRotX(const float& x) { mRotation.x = x; }
	void setRotY(const float& y) { mRotation.y = y; }
	void setRotZ(const float& z) { mRotation.z = z; }

	float getRotX() { return mRotation.x; }
	float getRotY() { return mRotation.y; }
	float getRotZ() { return mRotation.z; }
#pragma endregion Rotation
};

