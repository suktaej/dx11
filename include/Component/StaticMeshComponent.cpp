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
	mMesh = dynamic_cast<CStaticMesh*>(mesh.getMesh(name));

    if (!mMesh)
        return;
}

void CStaticMeshComponent::setMesh(CMesh& mesh)
{
    mMesh = dynamic_cast<CStaticMesh*>(&mesh);
}

void CStaticMeshComponent::registMap()
{
    using namespace DirectX;
   
    XMMATRIX world = XMLoadFloat4x4(&mWorldMatrix);
    XMFLOAT4X4 transposed;
    XMStoreFloat4x4(&transposed, XMMatrixTranspose(world));

    mScene->setInstanceMap(getMesh(), transposed);

    CGraphicShader* meshShader = dynamic_cast<CGraphicShader*>(getShader());
    if (!meshShader)
        return;

    mScene->setShaderMap(getMesh(), meshShader);
}

bool CStaticMeshComponent::init()
{
    CMeshComponent::init();

    return true;
}

bool CStaticMeshComponent::init(const char* name)
{
    CMeshComponent::init(name);

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

    registMap();
}

void CStaticMeshComponent::render()
{
    CMeshComponent::render();

    mShader->setShader();
    // TODO : 인스턴싱 대상 구분변수(bool)할당
    //mMesh->render();
}

void CStaticMeshComponent::postRender()
{
    CMeshComponent::postRender();
}

std::unique_ptr<CComponent> CStaticMeshComponent::clone() const
{
    return std::make_unique<CStaticMeshComponent>(createKey(), *this);
}

