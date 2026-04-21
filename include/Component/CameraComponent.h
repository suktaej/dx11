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
	XMFLOAT4X4 mProjection;
	EProjectionType mProjectionType = EProjectionType::Perspective;
	const float mHorizontalFOV = 90.f;
	float mFieldOfView = 0.f;
	float mNear = 0.5f;
	float mFar = 1000.f;
	float mWidth = 0.f;
	float mHeight = 0.f;

private:
	void updateViewMatrix();
	void calcVerticalFOV();

public:
	void setProjectionType(EProjectionType type);
	void setAngle(float angle) { mFieldOfView = angle; }
	void setNear(float value) { mNear = value; }
	void setFar(float value) { mFar = value; }
	void setViewResolution(float width, float height) { mWidth = width, height = height; }
	void setCameraPosition(float x, float y, float z);
	void setCameraRotation(float x, float y, float z);
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

