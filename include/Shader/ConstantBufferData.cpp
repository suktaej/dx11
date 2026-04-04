#include "ConstantBufferData.h"
#include "ShaderManager.h"
#include "ConstantBuffer.h"

CConstantBufferData::CConstantBufferData()
{
}

CConstantBufferData::CConstantBufferData(const CConstantBufferData& data)
{
}

CConstantBufferData::CConstantBufferData(CConstantBufferData&& data) noexcept
{
}

CConstantBufferData::~CConstantBufferData()
{
}

void CConstantBufferData::setConstantBuffer(const std::string& name)
{
	mBuffer = mShaderMgr->findConstantBuffer(name);
}
