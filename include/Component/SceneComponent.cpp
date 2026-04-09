#include "SceneComponent.h"

CSceneComponent::CSceneComponent(ComponentKey key) 
    : CComponent(key),
	mLocalScale(1.f, 1.f, 1.f),
	mLocalPosition(0.f, 0.f, 0.f),
	mLocalRotation(0.f, 0.f, 0.f, 1.f)
{
    XMStoreFloat4x4(&mLocalMatrix, DirectX::XMMatrixIdentity());
    XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixIdentity());
}

CSceneComponent::CSceneComponent(ComponentKey key, const CSceneComponent& other) 
    : CComponent(key, other),
    mLocalScale(other.mLocalScale),
    mLocalPosition(other.mLocalPosition),
    mLocalRotation(other.mLocalRotation),
    mParent(nullptr),
    mIsLocalDirty(true),
    mIsWorldDirty(true)
{
    // 계층구조와 무관한 독립노드 생성
    //mChildList.clear();
    // 복사 후 다음 프레임에 정보갱신(IsDerty)
    //XMStoreFloat4x4(&mLocalMatrix, DirectX::XMMatrixIdentity());
    //XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixIdentity());
}

CSceneComponent::~CSceneComponent()
{
    // 부모의 자식 목록에서 나를 제거
    if (mParent) 
    {
        auto it = std::find(mParent->mChildList.begin(), mParent->mChildList.end(), this);
        
        if (it != mParent->mChildList.end()) 
            mParent->mChildList.erase(it);
    }
    // 내 자식들의 부모를 null로 설정
    for (auto* child : mChildList)
    {
        child->mParent = nullptr;
        child->invalidateTransform();
    }

    mChildList.clear();
}

void CSceneComponent::setLocalScale(const DirectX::XMFLOAT3& scale)
{
    mLocalScale = scale;
    mIsLocalDirty = true;

    // Eager
    //updateWorldTransform();
    // Lazy Evaluation
    invalidateTransform();
}

void CSceneComponent::setLocalScale(const EAxis& axis, const float& scale)
{
    using namespace DirectX;

    XMVECTOR curScale = XMLoadFloat3(&mLocalScale);

    switch (axis) 
    {
    case EAxis::x: curScale = XMVectorSetX(curScale, scale); break;
    case EAxis::y: curScale = XMVectorSetY(curScale, scale); break;
    case EAxis::z: curScale = XMVectorSetZ(curScale, scale); break;
    }

    XMStoreFloat3(&mLocalScale, curScale);

    mIsLocalDirty = true;
    invalidateTransform();
}

void CSceneComponent::setLocalRotation(const DirectX::XMFLOAT3& rotation)
{
    using namespace DirectX;

    XMVECTOR quat = XMQuaternionRotationRollPitchYaw(
            XMConvertToRadians(rotation.x),
            XMConvertToRadians(rotation.y),
            XMConvertToRadians(rotation.z));

    //mLocalRotation = rotation;
    XMStoreFloat4(&mLocalRotation, quat);
    mIsLocalDirty = true;

    //mEuler = rotation;
    //mEulerDirty = false;

    //updateWorldTransform();
    // Lazy Evaluation
    invalidateTransform();
}

void CSceneComponent::addLocalRotation(const EAxis& axis, const float& angle)
{
    using namespace DirectX;

    XMVECTOR curQuat = XMLoadFloat4(&mLocalRotation);
    XMVECTOR delta = XMQuaternionRotationAxis(FAxis::Get(axis), XMConvertToRadians(angle));
    curQuat = XMQuaternionMultiply(curQuat, delta);

    XMStoreFloat4(&mLocalRotation, curQuat);

    mIsLocalDirty = true;
    invalidateTransform();
}

void CSceneComponent::addWorldScale(const EAxis& axis, const float& scale)
{
    using namespace DirectX;

    XMFLOAT3 worldScale = getWorldScale();
    XMVECTOR worldScaleVec = XMLoadFloat3(&worldScale);
    XMVECTOR offset = XMVectorScale(FAxis::Get(axis), scale);
    XMVECTOR newScale = XMVectorAdd(worldScaleVec, offset);

    XMStoreFloat3(&worldScale, newScale);
    setWorldScale(worldScale);
}

void CSceneComponent::addWorldPosition(const EAxis& axis, const float& pos)
{
    using namespace DirectX;

    XMFLOAT3 worldPos = getWorldPosition();
    XMVECTOR worldPosVec = XMLoadFloat3(&worldPos);
    XMVECTOR offset = XMVectorScale(FAxis::Get(axis), pos);
    XMVECTOR newPos = XMVectorAdd(worldPosVec, offset);

    XMStoreFloat3(&worldPos, newPos);
    setWorldPosition(worldPos);
}

void CSceneComponent::setLocalPosition(const DirectX::XMFLOAT3& position)
{
    mLocalPosition = position;
    mIsLocalDirty = true;

    //updateWorldTransform();
    // Lazy Evaluation
    invalidateTransform();
}

void CSceneComponent::setLocalPosition(const EAxis& axis, const float& pos)
{
    using namespace DirectX;

    XMVECTOR curPos = XMLoadFloat3(&mLocalPosition);

    switch (axis)
    {
    case EAxis::x: curPos = XMVectorSetX(curPos, pos); break;
    case EAxis::y: curPos = XMVectorSetY(curPos, pos); break;
    case EAxis::z: curPos = XMVectorSetZ(curPos, pos); break;
    }

    XMStoreFloat3(&mLocalPosition, curPos);

    mIsLocalDirty = true;
    invalidateTransform();
}

void CSceneComponent::invalidateTransform()
{
    mIsWorldDirty = true;

    for (CSceneComponent* it : mChildList)
        it->invalidateTransform();
}

const DirectX::XMFLOAT4X4& CSceneComponent::getWorldMatrix()
{
	// Lazy Evaluation
    if(mIsWorldDirty)
        updateWorldTransform();

    return mWorldMatrix;
}

DirectX::XMFLOAT3 CSceneComponent::getWorldScale()
{
    using namespace DirectX;

    XMVECTOR scale, quat, trans;
    XMMatrixDecompose(&scale, &quat, &trans, XMLoadFloat4x4(&getWorldMatrix()));

    return XMFLOAT3(
        XMVectorGetX(scale), 
        XMVectorGetY(scale), 
        XMVectorGetZ(scale));
}

DirectX::XMFLOAT4 CSceneComponent::getWorldRotation()
{
    using namespace DirectX;

    XMVECTOR scale, quat, trans;
    XMMatrixDecompose(&scale, &quat, &trans, XMLoadFloat4x4(&getWorldMatrix()));

    XMFLOAT4 result;
    XMStoreFloat4(&result, quat);
    return result;
}

DirectX::XMFLOAT3 CSceneComponent::getWorldPosition()
{
    const DirectX::XMFLOAT4X4& matWorld = getWorldMatrix();
    return DirectX::XMFLOAT3(matWorld._41, matWorld._42, matWorld._43);
}

void CSceneComponent::updateWorldTransform()
{
    using namespace DirectX;

    if (mIsLocalDirty)
	{
		XMMATRIX matScale = XMMatrixScaling(mLocalScale.x, mLocalScale.y, mLocalScale.z);
        XMVECTOR vRot = XMQuaternionNormalize(XMLoadFloat4(&mLocalRotation));
		XMMATRIX matRot = XMMatrixRotationQuaternion(vRot);
		XMMATRIX matPos = XMMatrixTranslation(mLocalPosition.x, mLocalPosition.y, mLocalPosition.z);

		XMStoreFloat4x4(&mLocalMatrix, matScale * matRot * matPos);
		mIsLocalDirty = false;
	}

    if (mParent)
    {
        XMMATRIX matWorld = XMLoadFloat4x4(&mLocalMatrix) * XMLoadFloat4x4(&mParent->getWorldMatrix());		
        XMStoreFloat4x4(&mWorldMatrix, matWorld);
    }
    else
        mWorldMatrix = mLocalMatrix;

    mIsWorldDirty = false;

    // Eager Update
	// 자식들에게 전파(재귀)
    // 부모의 mWorldMatrix가 갱신되었으므로 자식들은 이를 기반으로 즉시 계산
    //for (CSceneComponent* it : mChildList)
    //    it->updateWorldTransform();
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

        mLocalScale.x = (std::abs(pScale.x) > 1e-6f) ? scale.x / pScale.x : 0.f;
        mLocalScale.y = (std::abs(pScale.y) > 1e-6f) ? scale.y / pScale.y : 0.f;
        mLocalScale.z = (std::abs(pScale.z) > 1e-6f) ? scale.z / pScale.z : 0.f;
    }
    else
        mLocalScale = scale;

    mIsLocalDirty = true;
    invalidateTransform();
    //updateWorldTransform();
}

void CSceneComponent::setWorldScale(const EAxis& axis, const float& scale)
{
    using namespace DirectX;

    XMFLOAT3 worldScale = getWorldScale();
    XMVECTOR worldScaleVec = XMLoadFloat3(&worldScale);

    switch (axis)
    {
    case EAxis::x: worldScaleVec = XMVectorSetX(worldScaleVec, scale); break;
    case EAxis::y: worldScaleVec = XMVectorSetY(worldScaleVec, scale); break;
    case EAxis::z: worldScaleVec = XMVectorSetZ(worldScaleVec, scale); break;
    }

    XMStoreFloat3(&worldScale, worldScaleVec);
    setWorldScale(worldScale);
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
        //XMVECTOR parentScale, parentQuat, parentTrans;
        //XMMatrixDecompose(&parentScale, &parentQuat, &parentTrans, XMLoadFloat4x4(&mParent->getWorldMatrix()));
        //parentQuat = XMQuaternionNormalize(parentQuat);

        // 로컬 회전 계산: LocalQuat = WorldQuat * Inverse(ParentWorldQuat)
        XMFLOAT4 parentWorldRot = mParent->getWorldRotation();
        XMVECTOR parentQuat = XMLoadFloat4(&parentWorldRot);
        XMVECTOR invParentQuat = XMQuaternionInverse(parentQuat);
        // 행렬곱 순서 주의
        XMVECTOR localQuat = XMQuaternionMultiply(worldQuat, invParentQuat);
        localQuat = XMQuaternionNormalize(localQuat);

        XMStoreFloat4(&mLocalRotation, localQuat);
    }
    else
    {
        worldQuat = XMQuaternionNormalize(worldQuat);
        XMStoreFloat4(&mLocalRotation, worldQuat);
    }

    mIsLocalDirty = true;
    //mEulerDirty = true;
	invalidateTransform();
    //updateWorldTransform();
}

void CSceneComponent::addLocalScale(const EAxis& axis, const float& scale)
{
    using namespace DirectX;

    XMVECTOR curScale = XMLoadFloat3(&mLocalScale);
    XMVECTOR offset = XMVectorScale(FAxis::Get(axis), scale);
    XMVECTOR newScale = XMVectorAdd(curScale, offset);
    XMStoreFloat3(&mLocalScale, newScale);

    mIsLocalDirty = true;
    invalidateTransform();
}

void CSceneComponent::addLocalPosition(const EAxis& axis, const float& pos)
{
    using namespace DirectX;

    XMVECTOR curPos = XMLoadFloat3(&mLocalPosition);
    XMVECTOR offset = XMVectorScale(FAxis::Get(axis), pos);
    XMVECTOR newPos = XMVectorAdd(curPos, offset);
    XMStoreFloat3(&mLocalPosition, newPos);

    mIsLocalDirty = true;
    invalidateTransform();
}

void CSceneComponent::addWorldRotation(const EAxis& axis, const float& angle)
{
    using namespace DirectX;

    XMFLOAT4 worldRot = getWorldRotation();
    XMVECTOR worldQuat = XMLoadFloat4(&worldRot);
    XMVECTOR delta = XMQuaternionRotationAxis(FAxis::Get(axis), XMConvertToRadians(angle));
    worldQuat = XMQuaternionMultiply(worldQuat, delta);

    if (mParent)
    {
        // 로컬 회전 계산: LocalQuat = WorldQuat * Inverse(ParentWorldQuat)
        XMFLOAT4 parentWorldRot = mParent->getWorldRotation();
        XMVECTOR parentQuat = XMLoadFloat4(&parentWorldRot);
        XMVECTOR invParentQuat = XMQuaternionInverse(parentQuat);
        // 행렬곱 순서 주의
        XMVECTOR localQuat = XMQuaternionMultiply(worldQuat, invParentQuat);
        localQuat = XMQuaternionNormalize(localQuat);

        XMStoreFloat4(&mLocalRotation, localQuat);
    }
    else
    {
        worldQuat = XMQuaternionNormalize(worldQuat);
        XMStoreFloat4(&mLocalRotation, worldQuat);
    }

    mIsLocalDirty = true;
    invalidateTransform();
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

        XMStoreFloat3(&mLocalPosition, localPos);
    }
    else
        mLocalPosition = position;

    mIsLocalDirty = true;
    invalidateTransform();
    //updateWorldTransform();
}

void CSceneComponent::setWorldPosition(const EAxis& axis, const float& pos)
{
    using namespace DirectX;

    XMFLOAT3 worldPos = getWorldPosition();
    XMVECTOR worldPosVec = XMLoadFloat3(&worldPos);

    switch (axis)
    {
    case EAxis::x: worldPosVec = XMVectorSetX(worldPosVec, pos); break;
    case EAxis::y: worldPosVec = XMVectorSetY(worldPosVec, pos); break;
    case EAxis::z: worldPosVec = XMVectorSetZ(worldPosVec, pos); break;
    }

    XMStoreFloat3(&worldPos, worldPosVec);
    setWorldPosition(worldPos);
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
    // 자기 자신이거나 순환한다면 중복방지
    if (&child == this || child.isDescendant(this))
        return;

    // 이미 내 자식인지 확인
    auto it = std::find(mChildList.begin(), mChildList.end(), &child);
    
    if (it != mChildList.end()) 
        return;

    // 이미 자식에 부모가 존재한다면 
    // 부모에서 자식을 제거
    if (child.mParent)
    {
        auto& siblings = child.mParent->mChildList;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), &child), siblings.end());
    }

    child.mParent = this;
    mChildList.emplace_back(&child);

    //child.mIsLocalDirty = true;
    child.invalidateTransform();
}

bool CSceneComponent::isDescendant(CSceneComponent* node)
{
    for (auto* p = mParent; p != nullptr; p = p->mParent)
        if (p == node)
            return true;

    return false;
}

//void CSceneComponent::detachFromParent()
//{
//    if (mParent)
//    {
//        auto& siblings = mParent->mChildList;
//        siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
//        mParent = nullptr;
//        invalidateTransform();
//    }
//}

//CSceneComponent::CSceneComponent(ComponentKey key, CSceneComponent&& other) noexcept
//	: CComponent(key, std::move(other)),
//	mLocalScale(other.mLocalScale),
//	mLocalRotation(other.mLocalRotation),
//	mLocalPosition(other.mLocalPosition),
//	mLocalMatrix(other.mLocalMatrix),
//	mWorldMatrix(other.mWorldMatrix),
//    mIsLocalDirty(other.mIsLocalDirty),
//    mIsWorldDirty(other.mIsWorldDirty)
//{
//    mParent = other.mParent;
//
//    if (mParent)
//    {
//        auto& siblings = mParent->mChildList;
//        auto it = std::find(siblings.begin(), siblings.end(), &other);
//        if (it != siblings.end())
//            *it = this;
//    }
//
//	mChildList = std::move(other.mChildList);
//
//	for (auto* child : mChildList)
//		child->mParent = this;
//
//	using namespace DirectX;
//
//	other.mLocalScale = XMFLOAT3(1.f, 1.f, 1.f);
//	other.mLocalPosition = XMFLOAT3(0.f, 0.f, 0.f);
//	other.mLocalRotation = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
//    XMStoreFloat4x4(&other.mLocalMatrix, DirectX::XMMatrixIdentity());
//    XMStoreFloat4x4(&other.mWorldMatrix, DirectX::XMMatrixIdentity());
//    other.mParent = nullptr;
//    other.mIsLocalDirty = true;
//    other.mIsWorldDirty = true;
//}

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

