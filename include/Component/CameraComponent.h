#pragma once
#include "SceneComponent.h"
using namespace DirectX;

enum class EProjectionType
{
	Perspective,
	Ortho
};

class CCameraComponent :
	public CSceneComponent
{
public:
	CCameraComponent(ComponentKey key);
	CCameraComponent(ComponentKey key, const CCameraComponent& other);
	~CCameraComponent() override;

private:

	XMFLOAT4X4 mView;
	//XMFLOAT3 mForward;
	//XMFLOAT3 mRight;
	//XMFLOAT3 mUp;

	XMFLOAT4X4 mProjection;
	EProjectionType mProjectionType = EProjectionType::Perspective;
	float mViewAngle = 90.f;
	float mNear = 1.f;
	float mFar = 1000.f;
	float mWidth = 0.f;
	float mHeight = 0.f;

private:
	void makeViewMat();

public:
	void setProjectionType(EProjectionType type);
	void setAngle(float angle) { mViewAngle = angle; }
	void setNear(float value) { mNear = value; }
	void setFar(float value) { mFar = value; }
	void setViewResolution(float width, float height) { mWidth = width, height = height; }
	const XMFLOAT4X4& getViewMat() const { return mView; }
	const XMFLOAT4X4& getProjMat() const { return mProjection; }

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
};

