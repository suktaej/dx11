#include "ColorMeshShader.h"
#include "../DeviceManager.h"

CColorMeshShader::CColorMeshShader()
{
}

CColorMeshShader::~CColorMeshShader()
{
}

bool CColorMeshShader::init(CDeviceManager& device)
{
	mDevice = device.getDevice();
	mContext = device.getContext();

    if(!createVertexShader(TEXT("Mesh.hlsl"), "ColorMeshVS", "vs_5_0"))
		return false;

    if(!createPixelShader(TEXT("Mesh.hlsl"), "ColorMeshPS", "ps_5_0"))
		return false;

	AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputLayoutDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT4), D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!createInputLayout(mVertexShaderBlob.Get()))
		return false;

    return true;
}
