#include "ShaderManager.h"
#include "Shader.h"
#include "../DeviceManager.h"
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

	// Shader
	createShader<CColorMeshShader>("TestShader");
	// Constant Buffer
	createConstantBuffer("Transform", sizeof(FTransformConstantBufferInfo), 0, EShaderBufferType::Vertex);

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
	if (findConstantBuffer(name))
		return true;

	std::unique_ptr<CConstantBuffer> CBuffer = std::make_unique<CConstantBuffer>();

	if (!CBuffer->init(mDeviceMgr, bufferType, size, registerSlot))
		return false;

	mConstantBufferMap.emplace(std::make_pair(name, std::move(CBuffer)));

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
