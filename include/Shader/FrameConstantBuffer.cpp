#include "FrameConstantBuffer.h"
CFrameConstantBuffer::CFrameConstantBuffer()
{
}

CFrameConstantBuffer::CFrameConstantBuffer(const CFrameConstantBuffer& data)
	: CConstantBufferData(data),
	mData(data.mData)
{
}

CFrameConstantBuffer::~CFrameConstantBuffer()
{
}

bool CFrameConstantBuffer::init()
{
	setConstantBuffer("FrameCB");
	return true;
}

void CFrameConstantBuffer::updateBuffer()
{
	// TODO : Scene, Camera update이후 constant Buffer 호출 수정
	// 현재 임시로 object constant buffer에서 받아와 사용 중
	
	// 전송용 임시 구조체 생성
	FFrameConstantBufferInfo frameData;

	// 연산은 Row-Major로 수행 (CPU 방식)
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&mData.View);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&mData.Projection);
	DirectX::XMMATRIX vp = view * proj;

	// GPU에 저장하기 위해 Transpose (GPU 방식-HLSL-인 Column-Major로 변환)
	DirectX::XMStoreFloat4x4(&frameData.View, DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&frameData.Projection, DirectX::XMMatrixTranspose(proj));
	DirectX::XMStoreFloat4x4(&frameData.VP, DirectX::XMMatrixTranspose(vp));
	mBuffer->updateBuffer(&frameData);
}

std::unique_ptr<CConstantBufferData> CFrameConstantBuffer::clone() const
{
	return std::make_unique<CFrameConstantBuffer>(*this);
}

std::unique_ptr<CFrameConstantBuffer> CFrameConstantBuffer::cloneFrame() const
{
	return std::make_unique<CFrameConstantBuffer>(*this);
}


