#include "TransformConstantBuffer.h"

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
	FTransformConstantBufferInfo sendData;

	// 연산은 Row-Major로 수행 (CPU 방식)
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mData.World);
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&mData.View);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&mData.Projection);
	DirectX::XMMATRIX wvp = world * view * proj;

	// GPU에 저장하기 위해 Transpose (GPU 방식-HLSL-인 Column-Major로 변환)
	DirectX::XMStoreFloat4x4(&sendData.World, DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&sendData.View, DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&sendData.Projection, DirectX::XMMatrixTranspose(proj));
	DirectX::XMStoreFloat4x4(&sendData.WVP, DirectX::XMMatrixTranspose(wvp));

	mBuffer->updateBuffer(&sendData);
}

std::unique_ptr<CConstantBufferData> CTransformConstantBuffer::clone() const
{
	return std::make_unique<CTransformConstantBuffer>(*this);
}

std::unique_ptr<CTransformConstantBuffer> CTransformConstantBuffer::cloneTransform() const
{
	return std::make_unique<CTransformConstantBuffer>(*this);
}
