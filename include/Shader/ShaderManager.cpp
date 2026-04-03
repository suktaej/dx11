#include "ShaderManager.h"
#include "../DeviceManager.h"
#include "Shader.h"
#include "ColorMeshShader.h"
#include "ConstantBuffer.h"

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
}

bool CShaderManager::init(CDeviceManager& device)
{
	mDeviceMgr = &device;

	createShader<CColorMeshShader>("TestShader");

	return true;
}

CShader* CShaderManager::findShader(const std::string& name)
{
	auto it = mShaderMap.find(name);

	if (it != mShaderMap.end())
		return it->second.get();
	
	return nullptr;
}

void CShaderManager::releaseShader(const std::string& name)
{
	auto it = mShaderMap.find(name);

	if (it != mShaderMap.end())
		mShaderMap.erase(it);
}

bool CShaderManager::createConstantBuffer(const std::string& name, int size, int registerSlot, EShaderBufferType bufferType)
{
	CConstantBuffer* CBuffer = findConstantBuffer(name);

	if (CBuffer)
		return true;

	CBuffer = new CConstantBuffer;

	if (!CBuffer->init(mDeviceMgr, bufferType, size, registerSlot))
		return false;

	mConstantBufferMap.insert(std::make_pair(name, CBuffer));

	return true;
}

CConstantBuffer* CShaderManager::findConstantBuffer(const std::string& name)
{
	auto it = mConstantBufferMap.find(name);

	if (it != mConstantBufferMap.end())
		return it->second.get();

	return nullptr;
}

void CShaderManager::releaseConstantBuffer(const std::string& name)
{
	auto it = mConstantBufferMap.find(name);

	if (it != mConstantBufferMap.end())
		mConstantBufferMap.erase(it);
}
