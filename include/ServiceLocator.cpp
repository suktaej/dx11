#include "ServiceLocator.h"

IDevice* CServiceLocator::mDevice = nullptr;
IAsset* CServiceLocator::mAsset = nullptr;
IMesh* CServiceLocator::mMesh = nullptr;
IShader* CServiceLocator::mShader = nullptr;
IGame* CServiceLocator::mGame = nullptr;