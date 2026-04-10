#include "MeshComponent.h"
#include "../Shader/TransformConstantBuffer.h"
#include "../ServiceLocator.h"

CMeshComponent::CMeshComponent(ComponentKey key) : CSceneComponent(key)
{
    mTransformConstantBuffer = std::make_unique<CTransformConstantBuffer>();
}

CMeshComponent::CMeshComponent(ComponentKey key, const CMeshComponent& other) : CSceneComponent(key, other)
{
    auto cloned = other.mTransformConstantBuffer->clone();

    mTransformConstantBuffer = std::unique_ptr<CTransformConstantBuffer>(
        static_cast<CTransformConstantBuffer*>(cloned.release()));
}

CMeshComponent::~CMeshComponent()
{
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

    IDevice& device = CServiceLocator::getDevice();

    mTransformConstantBuffer->setWorld(mWorldMatrix);

    // 임시자료
    DirectX::XMFLOAT4X4 view, projection;
    // view
    DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
    DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
    DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixLookAtLH(eye, at, up));
    // projection
    RECT rc = { 0, 0, 1280, 720 };
    DirectX::XMStoreFloat4x4(&projection, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), (float)rc.right / rc.bottom, 0.1f, 100.0f));

    mTransformConstantBuffer->setView(view);
    mTransformConstantBuffer->setProjection(projection);
    mTransformConstantBuffer->updateBuffer(device.getContext());
}

void CMeshComponent::postRender()
{
    CSceneComponent::postRender();
}

std::unique_ptr<CComponent> CMeshComponent::clone() const
{
    return std::make_unique<CMeshComponent>(createKey(), *this);
}
