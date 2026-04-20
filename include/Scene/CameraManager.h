#pragma once
#include "../GameInfo.h"
#include "../ServiceInterface.h"
using namespace DirectX;

class CCameraManager : public ICamera
{
public:
	class CameraKey
	{
		friend class CScene;
		CameraKey() {}
	};
	CCameraManager(CameraKey key);
	~CCameraManager();

private:
	class CCameraComponent* mViewTarget;

public:
	void setViewTarget(class CCameraComponent* target) { mViewTarget = target; }
	class CCameraComponent* getVIewTarget() const { return mViewTarget; }
	const XMFLOAT4X4& getViewMat() const override;
	const XMFLOAT4X4& getProjMat() const override;

public:
	bool init();
	void update(float dt);

};

