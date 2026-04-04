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
	void setConstantBuffer(const std::string& name);

public:
	virtual bool init(CShaderManager& shaderMgr) = 0;
	virtual void updateBuffer(ID3D11DeviceContext* context) = 0;
	virtual std::unique_ptr<CConstantBufferData> clone() const = 0;
};

