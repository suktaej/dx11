#include "TransformConstantBuffer.h"

CTransformConstantBuffer::CTransformConstantBuffer()
{
}

CTransformConstantBuffer::CTransformConstantBuffer(const CTransformConstantBuffer& data)
{
}

CTransformConstantBuffer::CTransformConstantBuffer(CTransformConstantBuffer&& data)
{
}

CTransformConstantBuffer::~CTransformConstantBuffer()
{
}

bool CTransformConstantBuffer::init()
{
	return true;
}

void CTransformConstantBuffer::updateBuffer(ID3D11DeviceContext* context)
{
}

CTransformConstantBuffer* CTransformConstantBuffer::clone() const
{
	return nullptr;
}
