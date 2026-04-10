#pragma once
#include "ServiceInterface.h"

class CServiceLocator
{
public:
	static IDevice& getDevice() { return *mDevice; }
	static void provideDevice(IDevice& device) { mDevice = &device; }
	static IAsset& getAsset() { return *mAsset; }
	static void provideAsset(IAsset& asset) { mAsset = &asset; }
	static IMesh& getMesh() { return *mMesh; }
	static void provideMesh(IMesh& mesh) { mMesh = &mesh; }
	static IShader& getShader() { return *mShader; }
	static void provideShader(IShader& shader) { mShader = &shader; }

private:
	static IDevice* mDevice;
	static IAsset* mAsset;
	static IMesh* mMesh;
	static IShader* mShader;
};
