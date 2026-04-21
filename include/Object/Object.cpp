#include "Object.h"
#include "../ServiceLocator.h"
#include "../Asset/Mesh/Mesh.h"
#include "../Component/StaticMeshComponent.h"
#include "../Shader/GraphicShader.h"

CObject::CObject(ObjectKey key)
{
}

CObject::CObject(ObjectKey key, const CObject& other)
{
}

CObject::CObject(ObjectKey key, CObject&& other) noexcept
{
}

CObject::~CObject()
{
}

bool CObject::init(class CScene* scene)
{
	mScene = scene;
	return true;
}

bool CObject::init(class CScene* scene,const char* filePath)
{
	mScene = scene;
	return true;
}

void CObject::prevUpdate(float dt)
{
	mRootComponent->preUpdate(dt);
	nonSceneCompUpdate(&CComponent::preUpdate,dt);
}

void CObject::update(float dt)
{
	mRootComponent->update(dt);
	nonSceneCompUpdate(&CComponent::update, dt);

	lifeTimer(dt);
}

void CObject::postUpdate(float dt)
{
	mRootComponent->postUpdate(dt);
	nonSceneCompUpdate(&CComponent::postUpdate, dt);
}

void CObject::collision(float dt)
{
	mRootComponent->collision(dt);
	nonSceneCompUpdate(&CComponent::collision, dt);
}

void CObject::prevRender()
{
	mRootComponent->preRender();
	nonSceneCompUpdate(&CComponent::preRender);
}

void CObject::render()
{
	mRootComponent->render();
	nonSceneCompUpdate(&CComponent::render);
}

void CObject::postRender()
{
	mRootComponent->postRender();
	nonSceneCompUpdate(&CComponent::postRender);
}

void CObject::destroy()
{
	mIsActive = false;

	//auto sit = mSceneCompList.begin();
	//while (sit != mSceneCompList.end())
	//	(*sit++)->setActive(false);

	mSceneCompList.clear();

	//auto nit = mNonSceneCompList.begin();
	//while (nit != mNonSceneCompList.end())
	//	(*nit++)->setActive(false);

	mNonSceneCompList.clear();
}

void CObject::componentCleanUp()
{
	sceneCompCleanUp();
	nonSceneCompCleanUp();
}

void CObject::sceneCompCleanUp()
{
	auto it = mSceneCompList.begin();

	while (it != mSceneCompList.end())
	{
		if (!(*it)->isActive())
		{
			it = mSceneCompList.erase(it);
			continue;
		}
		else if (!(*it)->isEnable())
		{
			++it;
			continue;
		}

		++it;
	}
}

void CObject::nonSceneCompCleanUp()
{
	auto it = mNonSceneCompList.begin();

	while (it != mNonSceneCompList.end())
	{
		if (!(*it)->isActive())
		{
			it = mNonSceneCompList.erase(it);
			continue;
		}
		else if (!(*it)->isEnable())
		{
			++it;
			continue;
		}

		++it;
	}
}

void CObject::lifeTimer(float dt)
{
	if (mLifeTime > 0.f)
	{
		mLifeTime -= dt;

		if (mLifeTime <= 0)
			destroy();
	}
}

CComponent* CObject::findComponentByName(const std::string& name)
{
	auto nit = mNonSceneCompList.begin();

	while (nit != mNonSceneCompList.end())
	{
		if (name == (*nit).get()->getName())
			return (*nit).get();

		++nit;
	}

	auto sit = mSceneCompList.begin();

	while (sit != mSceneCompList.end())
	{
		if (name == (*sit).get()->getName())
			return (*sit).get();

		++sit;
	}

	return nullptr;
}

void CObject::makeStaticMeshBatchList(
	std::unordered_map<class CStaticMesh*, std::vector<DirectX::XMFLOAT4X4>>& instanceMap,
	std::unordered_map<CStaticMesh*, CGraphicShader*>& shaderMap)
{
	using namespace DirectX;

	for (auto& it : mSceneCompList)
	{
		CStaticMeshComponent* meshComp = dynamic_cast<CStaticMeshComponent*>(it.get());

		if (!meshComp)
			continue;

		CStaticMesh* mesh = meshComp->getMesh();
		//instanceMap[mesh].push_back(meshComp->getWorldMatrix());
		XMMATRIX world = XMLoadFloat4x4(&meshComp->getWorldMatrix());
		XMFLOAT4X4 transposed;
		XMStoreFloat4x4(&transposed, XMMatrixTranspose(world));
		instanceMap[mesh].push_back(transposed);

		// TODO : Shader Type Á¤¸®
		CGraphicShader* meshShader = dynamic_cast<CGraphicShader*>(meshComp->getShader());
		if (!meshShader)
			continue;
		shaderMap[mesh] = meshShader;
	}
}
