#include "MeshManager.h"
#include "../../DeviceManager.h"
#include "StaticMesh.h"

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

CMesh* CMeshManager::findMesh(const std::string& name)
{
    // std::unordered_map<std::string, std::unique_ptr<CMesh>>::iterator
    auto it = mMeshMap.find(name);

    if (it != mMeshMap.end()) 
        return it->second.get();
    
    return nullptr;
}
