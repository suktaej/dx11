#include "ObjectConstantBuffer.h"

CObjectConstantBuffer::CObjectConstantBuffer()
{
}

CObjectConstantBuffer::CObjectConstantBuffer(const CObjectConstantBuffer& data)
    : CConstantBufferData(data),
    mData(data.mData
    )
{
}

CObjectConstantBuffer::~CObjectConstantBuffer()
{
}

bool CObjectConstantBuffer::init()
{
	setConstantBuffer("ObjectCB");
    return false;
}

void CObjectConstantBuffer::updateBuffer()
{
	// 전송용 임시 구조체 생성
	FObjectConstantBufferInfo objectData;

	// 연산은 Row-Major로 수행 (CPU 방식)
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mData.World);
	DirectX::XMMATRIX vp = 
	DirectX::XMMATRIX wvp = world * vp;

	// GPU에 저장하기 위해 Transpose (GPU 방식-HLSL-인 Column-Major로 변환)
	DirectX::XMStoreFloat4x4(&objectData.World, DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&objectData.WVP, DirectX::XMMatrixTranspose(wvp));

	mBuffer->updateBuffer(&objectData);
}

std::unique_ptr<CConstantBufferData> CObjectConstantBuffer::clone() const
{
    return std::make_unique<CObjectConstantBuffer>(*this);
}

std::unique_ptr<CObjectConstantBuffer> CObjectConstantBuffer::cloneObject() const
{
    return std::make_unique<CObjectConstantBuffer>(*this);
}
