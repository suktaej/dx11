#pragma once
#include "ConstantBufferData.h"
class CFrameConstantBuffer :
    public CConstantBufferData
{
public:
	CFrameConstantBuffer();
	CFrameConstantBuffer(const CFrameConstantBuffer& data);
	//CFrameConstantBuffer(CFrameConstantBuffer&& data) noexcept;
	~CFrameConstantBuffer() override;

private:
	FFrameConstantBufferInfo mData;

public:
	bool init() override;
	void updateBuffer() override;
	std::unique_ptr<CConstantBufferData> clone() const override;
	std::unique_ptr<CFrameConstantBuffer> cloneFrame() const;

	void setView(const DirectX::XMFLOAT4X4& view) { mData.View = view; }
	void setProjection(const DirectX::XMFLOAT4X4& projection) { mData.Projection = projection; }
	void setVP(const DirectX::XMFLOAT4X4& vp) { mData.VP = vp; }
	//const FFrameConstantBufferInfo& getInfo() { return mData; }
};

