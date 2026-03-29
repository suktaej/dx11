#pragma once
#include "../GameInfo.h"
#include "../../DeviceManager.h"

class CMeshManager
{
	friend class CAssetManager;

public:
	CMeshManager();
	~CMeshManager();

private:
	CDeviceManager* mDeviceMgr = nullptr;
	std::unordered_map<std::string, class CMesh*> mMeshMap;

public:
	bool init(CDeviceManager& device);
};

