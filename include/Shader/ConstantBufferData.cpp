#include "ConstantBufferData.h"
#include "ShaderManager.h"
#include "ConstantBuffer.h"

CConstantBufferData::CConstantBufferData()
{
}

CConstantBufferData::CConstantBufferData(const CConstantBufferData& data)
{
	mBuffer = data.mBuffer;
}

CConstantBufferData::CConstantBufferData(CConstantBufferData&& data) noexcept
{
	mBuffer = data.mBuffer;
}

CConstantBufferData::~CConstantBufferData()
{
}

void CConstantBufferData::setConstantBuffer(const std::string& name)
{
	mBuffer = mShaderMgr->findConstantBuffer(name);
}
