#include "MeshComponent.h"
#include "../Shader/ObjectConstantBuffer.h"
#include "../ServiceLocator.h"

CMeshComponent::CMeshComponent(ComponentKey key) : CSceneComponent(key)
{
    mObjectCB = std::make_unique<CObjectConstantBuffer>();
    if (!mObjectCB->init())
        return;
}

CMeshComponent::CMeshComponent(ComponentKey key, const CMeshComponent& other) : CSceneComponent(key, other)
{
    std::unique_ptr<CConstantBufferData> cloned = other.mObjectCB->clone();
    
    // 성능 및 확신
    // 변환하고자 하는 타입임이 보장
    mObjectCB = std::unique_ptr<CObjectConstantBuffer>(
        static_cast<CObjectConstantBuffer*>(cloned.release()));

    if (!mObjectCB->init())
        return;
    
    // 안정성
    // dynamic_cast의 경우 런타임 오버해드 존재
    /*
    auto ptr = dynamic_cast<CObjectConstantBuffer*>(cloned.get());

    if (ptr)
    {
        cloned.release();
        mObjectCB.reset(ptr);
    }
    else
        assert(false && "Failed to cloned object to CObject Constant Buffer");
    */
}

CMeshComponent::~CMeshComponent()
{
}

void CMeshComponent::updateObjectBuffer()
{
    mObjectCB->setWorld(mWorldMatrix);
    mObjectCB->updateBuffer();
}

bool CMeshComponent::init(class CObject* obj)
{
    CSceneComponent::init(obj);

    return true;
}

bool CMeshComponent::init(class CObject* obj,const char* name)
{
    CSceneComponent::init(obj,name);

    return true;
}

void CMeshComponent::preUpdate(float dt)
{
    CSceneComponent::preUpdate(dt);
}

void CMeshComponent::update(float dt)
{
    CSceneComponent::update(dt);
}

void CMeshComponent::postUpdate(float dt)
{
    CSceneComponent::postUpdate(dt);
}

void CMeshComponent::collision(float dt)
{
    CSceneComponent::collision(dt);
}

void CMeshComponent::preRender()
{
    CSceneComponent::preRender();
}

void CMeshComponent::render()
{
    CSceneComponent::render();
    
#if MESHCALL_TYPE == 0 
    updateObjectBuffer();
#endif
}

void CMeshComponent::postRender()
{
    CSceneComponent::postRender();
}

std::unique_ptr<CComponent> CMeshComponent::clone() const
{
    return std::make_unique<CMeshComponent>(createKey(), *this);
}
