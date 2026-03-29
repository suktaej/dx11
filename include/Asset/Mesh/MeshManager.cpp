#include "MeshManager.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::init(CDeviceManager& device)
{
	mDeviceMgr = &device;

	return true;
}
