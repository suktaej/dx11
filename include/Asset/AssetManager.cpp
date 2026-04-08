#include "AssetManager.h"
#include "../DeviceManager.h"
#include "Mesh/MeshManager.h"

CAssetManager::CAssetManager()
{
}

CAssetManager::~CAssetManager()
{
}

bool CAssetManager::init(CDeviceManager& device)
{
	mDeviceMgr = &device;

	//mMeshMgr = std::make_unique<CMeshManager>();
	// 생성자를 계속 private / protected로 사용 시 new를 직접사용
	 mMeshMgr = std::unique_ptr<CMeshManager>(new CMeshManager());

	if(false == mMeshMgr->init(device))
		return false;

	return true;
}
