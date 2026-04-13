#include "TransformConstantBuffer.h"
#include "FrameConstantBuffer.h"

CTransformConstantBuffer::CTransformConstantBuffer()
{
}

CTransformConstantBuffer::CTransformConstantBuffer(const CTransformConstantBuffer& data)
	: CConstantBufferData(data),
	mData(data.mData)
{
}

CTransformConstantBuffer::CTransformConstantBuffer(CTransformConstantBuffer&& data) noexcept
	: CConstantBufferData(data),
	mData(data.mData)
{
}

CTransformConstantBuffer::~CTransformConstantBuffer()
{
}

bool CTransformConstantBuffer::init()
{
	setConstantBuffer("Transform");
	return true;
}

void CTransformConstantBuffer::updateBuffer()
{
	// 전송용 임시 구조체 생성
	//FTransformConstantBufferInfo sendData;
	FFrameConstantBufferInfo frameData;
	FObjectConstantBufferInfo objectData;

	// 연산은 Row-Major로 수행 (CPU 방식)
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mObjectData.World);
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&mFrameData.View);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&mFrameData.Projection);
	DirectX::XMMATRIX vp = view * proj;
	DirectX::XMMATRIX wvp = world * vp;

	// GPU에 저장하기 위해 Transpose (GPU 방식-HLSL-인 Column-Major로 변환)
	DirectX::XMStoreFloat4x4(&frameData.View, DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&frameData.Projection, DirectX::XMMatrixTranspose(proj));
	DirectX::XMStoreFloat4x4(&frameData.VP, DirectX::XMMatrixTranspose(vp));
	mBuffer->updateBuffer(&frameData);
	
	DirectX::XMStoreFloat4x4(&objectData.World, DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&objectData.WVP, DirectX::XMMatrixTranspose(wvp));
	mBuffer->updateBuffer(&objectData);
}

std::unique_ptr<CConstantBufferData> CTransformConstantBuffer::clone() const
{
	return std::make_unique<CTransformConstantBuffer>(*this);
}

std::unique_ptr<CTransformConstantBuffer> CTransformConstantBuffer::cloneTransform() const
{
	return std::make_unique<CTransformConstantBuffer>(*this);
}
