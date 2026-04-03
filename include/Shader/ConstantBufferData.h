#pragma once
#include "ConstantBuffer.h"

class CConstantBufferData abstract
{
public:
	CConstantBufferData();
	CConstantBufferData(const CConstantBufferData& data);
	CConstantBufferData(CConstantBufferData&& data) noexcept;
	virtual ~CConstantBufferData();

protected:
	class CShaderManager* mShaderMgr = nullptr;
	CConstantBuffer* mBuffer = nullptr;

protected:
	void setConstantBuffer(CShaderManager& shaderMgr, const std::string& name);

public:
	virtual bool init() = 0;
	virtual void updateBuffer(ID3D11DeviceContext* context) = 0;
	virtual CConstantBufferData* clone() const = 0;
};

