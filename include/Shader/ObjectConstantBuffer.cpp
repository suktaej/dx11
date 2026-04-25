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
    return true;
}

void CObjectConstantBuffer::updateBuffer()
{
	using namespace DirectX;

	// 전송용 임시 구조체 생성
	FObjectConstantBufferInfo objectData;

	XMMATRIX world = XMLoadFloat4x4(&mData.World);

	// GPU에 저장하기 위해 Transpose (GPU 방식-HLSL-인 Column-Major로 변환)
	XMStoreFloat4x4(&objectData.World, XMMatrixTranspose(world));

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
