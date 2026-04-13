#pragma once
#include "ConstantBufferData.h"
class CObjectConstantBuffer :
    public CConstantBufferData
{
public:
	CObjectConstantBuffer();
	CObjectConstantBuffer(const CObjectConstantBuffer& data);
	//CObjectConstantBuffer(CObjectConstantBuffer&& data) noexcept;
	~CObjectConstantBuffer() override;

private:
	FObjectConstantBufferInfo mData;

public:
	bool init() override;
	void updateBuffer() override;
	std::unique_ptr<CConstantBufferData> clone() const override;
	std::unique_ptr<CObjectConstantBuffer> cloneObject() const;

	void setWorld(const DirectX::XMFLOAT4X4& world) { mData.World = world; }
};
