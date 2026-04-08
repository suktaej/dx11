#pragma once
#include "Shader.h"

class CGraphicShader :
    public CShader
{
public:
	CGraphicShader(ShaderKey key);
	~CGraphicShader() override;

protected:
	ComPtr<ID3D11InputLayout> mInputLayout = nullptr;
	// Register 지정을 위한 구조체 정의
	std::vector<D3D11_INPUT_ELEMENT_DESC> mVecElementDescs;
	// ByteOffset을 멤버 변수로 선언하여 AddInputLayoutDesc 함수에서 입력 요소의 바이트 오프셋을 자동으로 계산하도록 함
	//UINT mByteOffset = 0;
	 
	// 셰이더 파일 경로를 셰이더 로딩 함수에서 기본 경로로 사용
	std::wstring mDefaultShaderFilePath = TEXT("../Shader/");
	// 각 셰이더 유형별로 ID3D11*Shader 포인터와 해당 셰이더의 컴파일된 코드(ID3DBlob)를 멤버 변수로 선언
	ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
	ComPtr<ID3DBlob> mVertexShaderBlob = nullptr;
	ComPtr<ID3D11HullShader> mHullShader = nullptr;
	ComPtr<ID3DBlob> mHullShaderBlob = nullptr;
	ComPtr<ID3D11DomainShader> mDomainShader = nullptr;
	ComPtr<ID3DBlob> mDomainShaderBlob = nullptr;
	ComPtr<ID3D11GeometryShader> mGeometryShader = nullptr;
	ComPtr<ID3DBlob> mGeometryShaderBlob = nullptr;
	ComPtr<ID3D11PixelShader> mPixelShader = nullptr;
	ComPtr<ID3DBlob> mPixelShaderBlob = nullptr;

public:
	//bool init() override;
	bool create(ID3D11Device* device) override;
	void setShader(ID3D11DeviceContext* context) final;
	void AddInputLayoutDesc(
	LPCSTR SemanticName,
	UINT SemanticIndex,
	DXGI_FORMAT Format,
	UINT InputSlot,
	UINT InputSize,
	D3D11_INPUT_CLASSIFICATION InputSlotClass,
	UINT InstanceDataStepRate);
	bool createInputLayout(ID3D11Device& device, ID3DBlob* vertexShaderBlob);
	bool compileShader(const std::wstring& fileName, const std::string& entryPoint, const std::string& shaderModel, ID3DBlob** shaderBlob);
	bool createVertexShader(ID3D11Device& device, const std::wstring& fileName, const std::string& entryPoint = "VSMain", const std::string& shaderModel = "vs_5_0");
	bool createHullShader(ID3D11Device& device, const std::wstring& fileName, const std::string& entryPoint = "HSMain", const std::string& shaderModel = "hs_5_0");
	bool createDomainShader(ID3D11Device& device, const std::wstring& fileName, const std::string& entryPoint = "DSMain", const std::string& shaderModel = "ds_5_0");
	bool createGeometryShader(ID3D11Device& device, const std::wstring& fileName, const std::string& entryPoint = "GSMain", const std::string& shaderModel = "gs_5_0");
	bool createPixelShader(ID3D11Device& device, const std::wstring& fileName, const std::string& entryPoint = "PSMain", const std::string& shaderModel = "ps_5_0");
};

