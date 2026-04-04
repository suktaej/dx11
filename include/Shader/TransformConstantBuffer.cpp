#include "TransformConstantBuffer.h"

CTransformConstantBuffer::CTransformConstantBuffer()
{
}

CTransformConstantBuffer::CTransformConstantBuffer(const CTransformConstantBuffer& data)
{
}

CTransformConstantBuffer::CTransformConstantBuffer(CTransformConstantBuffer&& data) noexcept
{
}

CTransformConstantBuffer::~CTransformConstantBuffer()
{
}

bool CTransformConstantBuffer::init(CShaderManager& shaderMgr)
{
	mShaderMgr = &shaderMgr;

	setConstantBuffer("Transform");
	return true;
}

void CTransformConstantBuffer::updateBuffer(ID3D11DeviceContext* context)
{
	// 연산은 Row-Major로 수행 (CPU 방식)
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mData.World);
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&mData.View);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&mData.Projection);
	DirectX::XMMATRIX wvp = world * view * proj;

	// GPU에 저장하기 위해 Transpose (GPU 방식-HLSL-인 Column-Major로 변환)
	DirectX::XMStoreFloat4x4(&mData.World, DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&mData.View, DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&mData.Projection, DirectX::XMMatrixTranspose(proj));
	DirectX::XMStoreFloat4x4(&mData.WVP, DirectX::XMMatrixTranspose(wvp));

	mBuffer->updateBuffer(&mData);
}

std::unique_ptr<CConstantBufferData> CTransformConstantBuffer::clone() const
{
	return std::make_unique<CTransformConstantBuffer>(*this);
}
