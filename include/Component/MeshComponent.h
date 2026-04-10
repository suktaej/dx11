#pragma once
#include "SceneComponent.h"

class CMeshComponent :
    public CSceneComponent
{
public:
	CMeshComponent(ComponentKey key);
	CMeshComponent(ComponentKey key, const CMeshComponent& other);
	//CMeshComponent(ComponentKey key, CMeshComponent&& other) noexcept;
	~CMeshComponent() override;

protected:
	std::unique_ptr<class CTransformConstantBuffer> mTransformConstantBuffer = nullptr;

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

