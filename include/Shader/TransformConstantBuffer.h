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
	//FTransformConstantBufferInfo mData;

public:
	bool init() override;
	void updateBuffer() override;
    std::unique_ptr<CConstantBufferData> clone() const override;
	std::unique_ptr<CTransformConstantBuffer> cloneTransform() const;

	void setWorld(const DirectX::XMFLOAT4X4& world) { mObjectData.World = world; }
	void setView(const DirectX::XMFLOAT4X4& view) { mFrameData.View = view; }
	void setProjection(const DirectX::XMFLOAT4X4& projection) { mFrameData.Projection = projection; }
};
