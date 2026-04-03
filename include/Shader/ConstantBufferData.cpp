#include "ConstantBufferData.h"
#include "ShaderManager.h"
#include "ConstantBuffer.h"

CConstantBufferData::CConstantBufferData()
{
}

CConstantBufferData::CConstantBufferData(const CConstantBufferData& data)
{

}

CConstantBufferData::CConstantBufferData(CConstantBufferData&& data)
{
	
}

CConstantBufferData::~CConstantBufferData()
{
}

void CConstantBufferData::setConstantBuffer(CShaderManager& shaderMgr, const std::string& name)
{
	mBuffer = shaderMgr.findConstantBuffer(name);
}
