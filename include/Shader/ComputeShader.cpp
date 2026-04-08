#include "ComputeShader.h"

CComputeShader::CComputeShader(ShaderKey key) : CShader(key)
{
}

CComputeShader::~CComputeShader()
{
}

bool CComputeShader::create(ID3D11Device* device)
{
    return true;
}

void CComputeShader::setShader(ID3D11DeviceContext* context)
{
}
