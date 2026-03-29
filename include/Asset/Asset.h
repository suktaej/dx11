#pragma once
#include "../GameInfo.h"

class CAsset abstract
{
public:
	CAsset();
	virtual ~CAsset();

protected:
    //class CScene* mScene = nullptr;
    std::string mName;
    EAssetType mAssetType;

public:
    const std::string& getName() const { return mName; } 
    void setName(const std::string& Name) { mName = Name; }
    EAssetType getAssetType() const { return mAssetType; }
};
