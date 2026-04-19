#pragma once
#include "Component.h"

class CMovementComponent :
	public CComponent
{
public:
	CMovementComponent(ComponentKey key);
	CMovementComponent(ComponentKey key, const CComponent& other);
	virtual ~CMovementComponent();

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

