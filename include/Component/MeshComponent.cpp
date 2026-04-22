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
    using namespace DirectX;
    
    mObjectCB->setWorld(mWorldMatrix);

    // hlsl에서 instancing을 사용하므로 VP정보만 활용
    // CPU 단계에서 WVP 갱신 불필요

    ICamera& camera = CServiceLocator::getCamera();

    XMMATRIX wvp =
        XMLoadFloat4x4(&mWorldMatrix) *
        XMLoadFloat4x4(&camera.getViewMat()) *
        XMLoadFloat4x4(&camera.getProjMat());

    XMFLOAT4X4 mwvp;
    XMStoreFloat4x4(&mwvp, wvp);

    mObjectCB->setWVP(mwvp);
    mObjectCB->updateBuffer();
}

bool CMeshComponent::init()
{
    CSceneComponent::init();

    return true;
}

bool CMeshComponent::init(const char* name)
{
    CSceneComponent::init(name);

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
    
    //updateObjectBuffer();
}

void CMeshComponent::postRender()
{
    CSceneComponent::postRender();
}

std::unique_ptr<CComponent> CMeshComponent::clone() const
{
    return std::make_unique<CMeshComponent>(createKey(), *this);
}
