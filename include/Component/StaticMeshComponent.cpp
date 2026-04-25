#include "StaticMeshComponent.h"
#include "../Asset/Mesh/StaticMesh.h"
#include "../Shader/GraphicShader.h"
#include "../Shader/ShaderManager.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Mesh/MeshManager.h"
#include "../Scene/Scene.h"
#include "../ServiceLocator.h"

CStaticMeshComponent::CStaticMeshComponent(ComponentKey key) : CMeshComponent(key)
{
}

CStaticMeshComponent::CStaticMeshComponent(ComponentKey key, const CStaticMeshComponent& other) : CMeshComponent(key, other)
{
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

void CStaticMeshComponent::setShader(const std::string& name)
{
    IShader& shader = CServiceLocator::getShader();
    mShader = shader.getShader(name);

    if (!mShader)
        return;
}

void CStaticMeshComponent::setShader(CShader& shader)
{
    mShader = &shader;
}

void CStaticMeshComponent::setMesh(const std::string& name)
{
    IMesh& mesh = CServiceLocator::getMesh();
	mMesh = mesh.getMesh(name);
}

void CStaticMeshComponent::setMesh(CMesh& mesh)
{
    mMesh = &mesh;
}



bool CStaticMeshComponent::init(class CObject* obj)
{
    CMeshComponent::init(obj);

    return true;
}

bool CStaticMeshComponent::init(class CObject* obj,const char* name)
{
    CMeshComponent::init(obj,name);

    return true;
}

void CStaticMeshComponent::preUpdate(float dt)
{
    CMeshComponent::preUpdate(dt);
}

void CStaticMeshComponent::update(float dt)
{
    CMeshComponent::update(dt);
}

void CStaticMeshComponent::postUpdate(float dt)
{
    CMeshComponent::postUpdate(dt);
}

void CStaticMeshComponent::collision(float dt)
{
    CMeshComponent::collision(dt);
}

void CStaticMeshComponent::preRender()
{
    CMeshComponent::preRender();

#if MESHCALL_TYPE == 1
    registMap();
#endif
}

void CStaticMeshComponent::render()
{
    CMeshComponent::render();

#if MESHCALL_TYPE == 0
    // TODO : 인스턴싱 대상 구분변수(bool)할당
    mShader->setShader();
    mMesh->render();
#endif
}

void CStaticMeshComponent::postRender()
{
    CMeshComponent::postRender();
}

std::unique_ptr<CComponent> CStaticMeshComponent::clone() const
{
    return std::make_unique<CStaticMeshComponent>(createKey(), *this);
}

#if MESHCALL_TYPE == 1
void CStaticMeshComponent::registMap()
{
    using namespace DirectX;

    XMMATRIX world = XMLoadFloat4x4(&mWorldMatrix);
    XMFLOAT4X4 transposed;
    XMStoreFloat4x4(&transposed, XMMatrixTranspose(world));

    CGraphicShader* meshShader = dynamic_cast<CGraphicShader*>(getShader());

    if (!meshShader)
        return;

	mScene->setInstanceBatch(mMesh, meshShader, transposed);
}
#endif
