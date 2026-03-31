#include "ShaderManager.h"
#include "Shader.h"
#include "../DeviceManager.h"

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
}
bool CShaderManager::init(CDeviceManager& device)
{
	mDeviceMgr = &device;

	return true;
}

CShader* CShaderManager::findShader(const std::string& name)
{
	auto it = mShaderMap.find(name);

	if (it != mShaderMap.end())
		return it->second.get();
	
	return nullptr;
}
