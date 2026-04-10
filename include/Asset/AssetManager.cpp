#include "AssetManager.h"
#include "../DeviceManager.h"
#include "Mesh/MeshManager.h"
#include "../ServiceLocator.h"

CAssetManager::CAssetManager()
{
}

CAssetManager::~CAssetManager()
{
}

void CAssetManager::serviceInit()
{
}

bool CAssetManager::init()
{
	//mMeshMgr = std::make_unique<CMeshManager>();
	// 생성자를 계속 private / protected로 사용 시 new를 직접사용
	 mMeshMgr = std::unique_ptr<CMeshManager>(new CMeshManager());
	 CServiceLocator::provideMesh(*mMeshMgr);

	if(false == mMeshMgr->init())
		return false;

	return true;
}
