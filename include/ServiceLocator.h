#pragma once
#include "ServiceInterface.h"

class CServiceLocator
{
public:
	static IGame& getGame() { return *mGame; }
	static void provideGame(IGame& game) { mGame = &game; }
	static IDevice& getDevice() { return *mDevice; }
	static void provideDevice(IDevice& device) { mDevice = &device; }
	static IAsset& getAsset() { return *mAsset; }
	static void provideAsset(IAsset& asset) { mAsset = &asset; }
	static IMesh& getMesh() { return *mMesh; }
	static void provideMesh(IMesh& mesh) { mMesh = &mesh; }
	static IShader& getShader() { return *mShader; }
	static void provideShader(IShader& shader) { mShader = &shader; }
	static IInput& getInput() { return *mInput; }
	static void provideInput(IInput& input) { mInput = &input; }
	static ICamera& getCamera() { return *mCamera; }
	static void provideCamera(ICamera& camera) { mCamera = &camera; }

private:
	static IGame* mGame;
	static IDevice* mDevice;
	static IAsset* mAsset;
	static IMesh* mMesh;
	static IShader* mShader;
	static IInput* mInput;
	static ICamera* mCamera;
};
