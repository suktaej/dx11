#include "ShaderManager.h"
#include "Shader.h"
#include "ColorMeshShader.h"
#include "ConstantBuffer.h"

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
}

bool CShaderManager::init()
{
	// Shader
	createShader<CColorMeshShader>("TestShader");
	// Constant Buffer
	// Per-Frame buffer (b0)
	createConstantBuffer("FrameCB", sizeof(FFrameConstantBufferInfo), 0, EShaderBufferType::Vertex);
	// Per-Object buffer (b3)
	createConstantBuffer("ObjectCB", sizeof(FObjectConstantBufferInfo), 3, EShaderBufferType::Vertex);

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

	std::unique_ptr<CConstantBuffer> CBuffer = std::make_unique<CConstantBuffer>(CConstantBuffer::ConstantBufferKey{});

	if (!CBuffer->init(bufferType, size, registerSlot))
		return false;

	mConstantBufferMap.emplace(std::make_pair(name, std::move(CBuffer)));

	return true;
}

CConstantBuffer* CShaderManager::findConstantBuffer(const std::string& name)
{
	if (this == nullptr) 
		// °´Ã¼°¡ »ý¼ºµÇÁö ¾Ê¾Ò°Å³ª ÀÌ¹Ì ÆÄ±«µÊ
		return nullptr;

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

void CShaderManager::serviceInit()
{
}

CShader* CShaderManager::getShader(const std::string& name)
{
	return findShader(name);
}

CConstantBuffer* CShaderManager::getConstant(const std::string& name)
{
	return findConstantBuffer(name);
}
