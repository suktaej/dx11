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

	mMeshMgr = std::make_unique<CMeshManager>();
	if(false == mMeshMgr->init(device))
		return false;
	return true;
}
