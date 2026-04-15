#pragma once
#include "../GameInfo.h"
#include "../ServiceInterface.h"

class CAssetManager : public IAsset
{
public:
	CAssetManager();
	~CAssetManager();

private:
	std::unique_ptr<class CMeshManager> mMeshMgr;

public:
	bool init();
	CMeshManager* getMeshManager() { return mMeshMgr.get(); }
};
