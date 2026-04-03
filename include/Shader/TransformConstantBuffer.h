#pragma once
#include "ConstantBufferData.h"

class CTransformConstantBuffer :
    public CConstantBufferData
{
public:
	CTransformConstantBuffer();
	CTransformConstantBuffer(const CTransformConstantBuffer& data);
    CTransformConstantBuffer(CTransformConstantBuffer&& data) noexcept;
	~CTransformConstantBuffer() override;

private:
	FTransformConstantBufferInfo mData;

public:
	bool init() override;
	void updateBuffer(ID3D11DeviceContext* context) override;
    CTransformConstantBuffer* clone() const override;
};
