#include "ColorMeshShader.h"
#include "../DeviceManager.h"

CColorMeshShader::CColorMeshShader(ShaderKey key) : CGraphicShader(key)
{
}

CColorMeshShader::~CColorMeshShader()
{
}

bool CColorMeshShader::create()
{
	CGraphicShader::create();

#if MESHCALL_TYPE == 0
	if (!createVertexShader(TEXT("Mesh.hlsl"), "ColorMeshVS", "vs_5_0"))
		return false;

	if (!createPixelShader(TEXT("Mesh.hlsl"), "ColorMeshPS", "ps_5_0"))
		return false;
#elif MESHCALL_TYPE == 1
	if (!createVertexShader(TEXT("InstanceMesh.hlsl"), "ColorMeshVS", "vs_5_0"))
		return false;

	if (!createPixelShader(TEXT("InstanceMesh.hlsl"), "ColorMeshPS", "ps_5_0"))
		return false;
#endif

	AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputLayoutDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::XMFLOAT4), D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!createInputLayout(mVertexShaderBlob.Get()))
		return false;

	return true;
}
