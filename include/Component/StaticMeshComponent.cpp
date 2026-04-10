#include "StaticMeshComponent.h"
#include "../Asset/Mesh/StaticMesh.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderManager.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Mesh/MeshManager.h"

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
}

void CStaticMeshComponent::setShader(CShader& shader)
{
}

void CStaticMeshComponent::setMesh(const std::string& name)
{
}

void CStaticMeshComponent::setMesh(CMesh& mesh)
{
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
}

void CStaticMeshComponent::render()
{
    CMeshComponent::render();

    mShader->setShader();
    mMesh->render();
}

void CStaticMeshComponent::postRender()
{
    CMeshComponent::postRender();
}

std::unique_ptr<CComponent> CStaticMeshComponent::clone() const
{
    return std::make_unique<CStaticMeshComponent>(createKey(), *this);
}

