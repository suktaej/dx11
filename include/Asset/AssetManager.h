#pragma once
#include "../GameInfo.h"

class CAssetManager
{
public:
	CAssetManager();
	~CAssetManager();

private:
	class CDeviceManager* mDeviceMgr = nullptr;
	std::unique_ptr<class CMeshManager> mMeshMgr;

public:
	bool init(CDeviceManager& device);
	CMeshManager* getMeshManager() { return mMeshMgr.get(); }
};
