#include "MeshComponent.h"
#include "../Shader/ObjectConstantBuffer.h"

CMeshComponent::CMeshComponent(ComponentKey key) : CSceneComponent(key)
{
    mObjectCB = std::make_unique<CObjectConstantBuffer>();
    mObjectCB->init();
}

CMeshComponent::CMeshComponent(ComponentKey key, const CMeshComponent& other) : CSceneComponent(key, other)
{
    std::unique_ptr<CConstantBufferData> cloned = other.mObjectCB->clone();
    
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

    // 성능 및 확신
    // 변환하고자 하는 타입임이 보장
    mObjectCB = std::unique_ptr<CObjectConstantBuffer>(
        static_cast<CObjectConstantBuffer*>(cloned.release()));
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
    using namespace DirectX;

    CSceneComponent::render();

    mObjectCB->setWorld(mWorldMatrix);

    // 임시자료
    XMFLOAT4X4 view, projection;
    // view
    XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
    XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
    XMStoreFloat4x4(&view, XMMatrixLookAtLH(eye, at, up));
    // projection
    RECT rc = { 0, 0, 1280, 720 };
    XMStoreFloat4x4(&projection, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), (float)rc.right / rc.bottom, 0.1f, 100.0f));

    XMMATRIX wvp = XMLoadFloat4x4(&mWorldMatrix) * XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection);
    XMFLOAT4X4 mwvp;
    XMStoreFloat4x4(&mwvp, wvp);

    mObjectCB->setWVP(mwvp);
    mObjectCB->updateBuffer();
    //XMMATRIX vp = mScene->GetMainCamera()->GetViewProjectionMatrix();

    //mTransformConstantBuffer->setView(view);
    //mTransformConstantBuffer->setProjection(projection);
    //mTransformConstantBuffer->updateBuffer();
}

void CMeshComponent::postRender()
{
    CSceneComponent::postRender();
}

std::unique_ptr<CComponent> CMeshComponent::clone() const
{
    return std::make_unique<CMeshComponent>(createKey(), *this);
}
