#include "Component.h"
#include "../Object/Object.h"
#include "../DeviceManager.h"

CComponent::CComponent(ComponentKey key) { }
CComponent::CComponent(ComponentKey key, const CComponent& other) { }
//CComponent::CComponent(ComponentKey key, CComponent&& other) noexcept { }
CComponent::~CComponent() { }

bool CComponent::init(class CObject* obj) 
{ 
	mOwner = obj;
	mScene = obj->getScene();

	return true; 
}

bool CComponent::init(class CObject* obj,const char* name)
{
	mOwner = obj;
	mScene = obj->getScene();

	return true;
}
void CComponent::preUpdate(float dt) {}
void CComponent::update(float dt) {}
void CComponent::postUpdate(float dt) {}
void CComponent::collision(float dt) {}
void CComponent::preRender() {}
void CComponent::render() {}
void CComponent::postRender() {}
std::unique_ptr<CComponent> CComponent::clone() const { return nullptr; }
void CComponent::eraseOwner() {}
