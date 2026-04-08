#include "SceneComponent.h"

CSceneComponent::CSceneComponent(ComponentKey key) 
    : CComponent(key)
	, mRelativeScale(1.f, 1.f, 1.f)
	, mRelativePosition(0.f, 0.f, 0.f)
	, mRelativeRotation(0.f, 0.f, 0.f, 1.f)
{
    XMStoreFloat4x4(&mLocalMatrix, DirectX::XMMatrixIdentity());
    XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixIdentity());
}

CSceneComponent::CSceneComponent(ComponentKey key, const CSceneComponent& other) 
    : CComponent(key, other)
    , mRelativeScale(1.f, 1.f, 1.f)
    , mRelativePosition(0.f, 0.f, 0.f)
    , mRelativeRotation(0.f, 0.f, 0.f, 1.f)
{
    XMStoreFloat4x4(&mLocalMatrix, DirectX::XMMatrixIdentity());
    XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixIdentity());
}

CSceneComponent::CSceneComponent(ComponentKey key, CSceneComponent&& other) noexcept
    : CComponent(key, std::move(other))
    , mRelativeScale(1.f, 1.f, 1.f)
    , mRelativePosition(0.f, 0.f, 0.f)
    , mRelativeRotation(0.f, 0.f, 0.f, 1.f)
{
    XMStoreFloat4x4(&mLocalMatrix, DirectX::XMMatrixIdentity());
    XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixIdentity());
}

CSceneComponent::~CSceneComponent()
{
}

void CSceneComponent::setRelativeScale(const DirectX::XMFLOAT3& scale)
{
    mRelativeScale = scale;
    mIsLocalDirty = true;
    updateWorldTransform();
    // Lazy Evaluation
    //invalidateTransform();
}

void CSceneComponent::setRelativeRotation(const DirectX::XMFLOAT3& rotation)
{
    using namespace DirectX;

    XMVECTOR quat = XMQuaternionRotationRollPitchYaw(
            XMConvertToRadians(rotation.x),
            XMConvertToRadians(rotation.y),
            XMConvertToRadians(rotation.z));

    //mRelativeRotation = rotation;
    XMStoreFloat4(&mRelativeRotation, quat);
    mIsLocalDirty = true;
    updateWorldTransform();
    // Lazy Evaluation
    //invalidateTransform();
}

void CSceneComponent::setRelativePosition(const DirectX::XMFLOAT3& position)
{
    mRelativePosition = position;
    mIsLocalDirty = true;
    updateWorldTransform();
    // Lazy Evaluation
    //invalidateTransform();
}

//void CSceneComponent::invalidateTransform()
//{
//    if (mIsWorldDirty)
//        return;
//
//    mIsWorldDirty = true;
//
//    for (CSceneComponent* it : mChildList)
//        it->invalidateTransform();
//}

const DirectX::XMFLOAT4X4& CSceneComponent::getWorldMatrix()
{
	// Lazy Evaluation
    //if(mIsWorldDirty)
    //    updateWorldTransform();

    return mWorldMatrix;
}

const DirectX::XMFLOAT3 CSceneComponent::getWorldScale()
{
    using namespace DirectX;

    XMVECTOR vX = XMVectorSet(mWorldMatrix._11, mWorldMatrix._12, mWorldMatrix._13, 0.0f);
    XMVECTOR vY = XMVectorSet(mWorldMatrix._21, mWorldMatrix._22, mWorldMatrix._23, 0.0f);
    XMVECTOR vZ = XMVectorSet(mWorldMatrix._31, mWorldMatrix._32, mWorldMatrix._33, 0.0f);

    return XMFLOAT3(XMVectorGetX(XMVector3Length(vX)),
        XMVectorGetX(XMVector3Length(vY)),
        XMVectorGetX(XMVector3Length(vZ)));
}

const DirectX::XMFLOAT4 CSceneComponent::getWorldRotation()
{
    using namespace DirectX;

    XMVECTOR scale, quat, trans;
    XMMatrixDecompose(&scale, &quat, &trans, XMLoadFloat4x4(&mWorldMatrix));

    XMFLOAT4 result;
    XMStoreFloat4(&result, quat);
    return result;
}

const DirectX::XMFLOAT3 CSceneComponent::getWorldPosition()
{
    return DirectX::XMFLOAT3(mWorldMatrix._41, mWorldMatrix._42, mWorldMatrix._43);
}

void CSceneComponent::updateWorldTransform()
{
    using namespace DirectX;

    if (mIsLocalDirty)
	{
		XMMATRIX matScale = XMMatrixScaling(mRelativeScale.x, mRelativeScale.y, mRelativeScale.z);
        XMVECTOR vRot = XMQuaternionNormalize(XMLoadFloat4(&mRelativeRotation));
		XMMATRIX matRot = XMMatrixRotationQuaternion(vRot);
		XMMATRIX matPos = XMMatrixTranslation(mRelativePosition.x, mRelativePosition.y, mRelativePosition.z);

		XMStoreFloat4x4(&mLocalMatrix, matScale * matRot * matPos);
		mIsLocalDirty = false;
	}

    XMMATRIX matLocal = XMLoadFloat4x4(&mLocalMatrix);
    XMMATRIX matWorld;

    if (mParent)
        matWorld = matLocal * XMLoadFloat4x4(&mParent->getWorldMatrix());
    else
        matWorld = matLocal;

    XMStoreFloat4x4(&mWorldMatrix, matWorld);
    mIsWorldDirty = false;

    // Eager Update
	// 자식들에게 전파(재귀)
    // 부모의 mWorldMatrix가 갱신되었으므로 자식들은 이를 기반으로 즉시 계산
    for (CSceneComponent* it : mChildList)
        it->updateWorldTransform();
}

void CSceneComponent::setWorldScale(const DirectX::XMFLOAT3& scale)
{
    using namespace DirectX;

    if (mParent)
    {
        // 부모의 월드 스케일을 가져옴
        XMVECTOR parentScale, parentQuat, parentTrans;
        XMMatrixDecompose(&parentScale, &parentQuat, &parentTrans, XMLoadFloat4x4(&mParent->getWorldMatrix()));

        // 로컬 스케일 계산: LocalScale = WorldScale / ParentWorldScale
        XMFLOAT3 pScale;
        XMStoreFloat3(&pScale, parentScale);

        mRelativeScale.x = scale.x / pScale.x;
        mRelativeScale.y = scale.y / pScale.y;
        mRelativeScale.z = scale.z / pScale.z;
    }
    else
        mRelativeScale = scale;

    mIsLocalDirty = true;
    updateWorldTransform();
}

void CSceneComponent::setWorldRotation(const DirectX::XMFLOAT3& rotation)
{
    using namespace DirectX;
	
    // 월드 회전을 쿼터니언으로 변환
	XMVECTOR worldQuat = XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(rotation.x),
		XMConvertToRadians(rotation.y),
		XMConvertToRadians(rotation.z));

    if (mParent)
    {
        // 부모의 월드 회전을 쿼터니언으로 가져옴
        XMVECTOR parentScale, parentQuat, parentTrans;
        XMMatrixDecompose(&parentScale, &parentQuat, &parentTrans, XMLoadFloat4x4(&mParent->getWorldMatrix()));

        // 로컬 회전 계산: LocalQuat = WorldQuat * Inverse(ParentWorldQuat)
        parentQuat = XMQuaternionNormalize(parentQuat);
        XMVECTOR invParentQuat = XMQuaternionInverse(parentQuat);
        XMVECTOR localQuat = XMQuaternionMultiply(worldQuat, invParentQuat);
        localQuat = XMQuaternionNormalize(localQuat);

        XMStoreFloat4(&mRelativeRotation, localQuat);
    }
    else
    {
        worldQuat = XMQuaternionNormalize(worldQuat);
        XMStoreFloat4(&mRelativeRotation, worldQuat);
    }

    mIsLocalDirty = true;
    updateWorldTransform();
}

void CSceneComponent::setWorldPosition(const DirectX::XMFLOAT3& position)
{
    using namespace DirectX;

    if (mParent)
    {
        // 부모의 월드 행렬의 역행렬을 구함
        XMMATRIX invParentWorld = XMMatrixInverse(nullptr, XMLoadFloat4x4(&mParent->getWorldMatrix()));

        // 월드 좌표를 부모의 로컬 공간으로 변환 (좌표 * 역행렬)
        XMVECTOR worldPos = XMLoadFloat3(&position);
        XMVECTOR localPos = XMVector3TransformCoord(worldPos, invParentWorld);

        XMStoreFloat3(&mRelativePosition, localPos);
    }
    else
        mRelativePosition = position;

    mIsLocalDirty = true;
    updateWorldTransform();
}

bool CSceneComponent::init()
{
    return true;
}

bool CSceneComponent::init(const char* name)
{
    return true;
}

void CSceneComponent::preUpdate(float dt)
{
    CComponent::preUpdate(dt);
}

void CSceneComponent::update(float dt)
{
    CComponent::update(dt);
}

void CSceneComponent::postUpdate(float dt)
{
    CComponent::postUpdate(dt);
}

void CSceneComponent::collision(float dt)
{
	CComponent::collision(dt);
}

void CSceneComponent::preRender()
{
    CComponent::preRender();
}

void CSceneComponent::render()
{
    CComponent::render();
}

void CSceneComponent::postRender()
{
    CComponent::postRender();
}

std::unique_ptr<CComponent> CSceneComponent::clone() const
{
    return std::make_unique<CSceneComponent>(createKey(), *this);
}

void CSceneComponent::eraseOwner()
{
}

void CSceneComponent::addChild(CSceneComponent& child)
{
    child.mParent = this;
	mChildList.emplace_back(&child);
}

//DirectX::XMFLOAT3 CSceneComponent::quaternionToEuler(DirectX::XMVECTOR q)
//{
//    using namespace DirectX;
//    XMFLOAT4 quat;
//    XMStoreFloat4(&quat, q);
//
//    float pitch, yaw, roll;
//
//    // Roll (X-axis rotation)
//    float sinr_cosp = 2.f * (quat.w * quat.x + quat.y * quat.z);
//    float cosr_cosp = 1.f - 2.f * (quat.x * quat.x + quat.y * quat.y);
//    pitch = std::atan2(sinr_cosp, cosr_cosp);
//
//    // Yaw (Y-axis rotation)
//    float sinp = 2.f * (quat.w * quat.y - quat.z * quat.x);
//    
//    if (std::abs(sinp) >= 0.9999f)
//        yaw = std::copysign(XM_PIDIV2, sinp);
//    else
//        yaw = std::asin(sinp);
//
//    // Pitch (Z-axis rotation)
//    float siny_cosp = 2.f * (quat.w * quat.z + quat.x * quat.y);
//    float cosy_cosp = 1.f - 2.f * (quat.y * quat.y + quat.z * quat.z);
//    roll = std::atan2(siny_cosp, cosy_cosp);
//
//    return {
//        XMConvertToDegrees(pitch),
//        XMConvertToDegrees(yaw),
//        XMConvertToDegrees(roll)
//    };
//}

