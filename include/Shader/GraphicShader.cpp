#include "GraphicShader.h"
#include "../DeviceManager.h"

CGraphicShader::CGraphicShader(ShaderKey key) : CShader(key)
{
}

CGraphicShader::~CGraphicShader()
{
}

bool CGraphicShader::create(ID3D11Device* device)
{
	return true;
}

void CGraphicShader::setShader(ID3D11DeviceContext* context)
{
	context->VSSetShader(mVertexShader.Get(), nullptr, 0);
	context->HSSetShader(mHullShader.Get(), nullptr, 0);
	context->DSSetShader(mDomainShader.Get(), nullptr, 0);
	context->GSSetShader(mGeometryShader.Get(), nullptr, 0);
	context->PSSetShader(mPixelShader.Get(), nullptr, 0);

	// IA : Input Assembler
	context->IASetInputLayout(mInputLayout.Get());
}

void CGraphicShader::AddInputLayoutDesc(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, UINT InputSlot, UINT InputSize, D3D11_INPUT_CLASSIFICATION InputSlotClass, UINT InstanceDataStepRate)
{
	//LPCSTR SemanticName : 입력 요소의 의미를 나타내는 문자열입니다. 예를 들어, "POSITION", "TEXCOORD", "NORMAL" 등이 있습니다. 이 문자열은 셰이더 코드에서 해당 입력 요소를 참조할 때 사용됩니다.
	//UINT SemanticIndex : 입력 요소의 의미가 여러 개일 때, 각 요소를 구분하기 위한 인덱스입니다. 예를 들어, "TEXCOORD0", "TEXCOORD1" 등으로 사용됩니다.
	//DXGI_FORMAT Format : 입력 요소의 데이터 형식을 지정하는 열거형입니다. 예를 들어, DXGI_FORMAT_R32G32B32_FLOAT는 3개의 32비트 부동 소수점 값으로 구성된 벡터를 나타냅니다.
	//UINT InputSlot : 입력 요소가 속한 입력 슬롯을 지정하는 정수입니다. 일반적으로 0부터 시작하여 여러 개의 슬롯을 사용할 수 있습니다. 각 슬롯은 별도의 버퍼에서 데이터를 가져올 수 있습니다.
	//UINT AlignedByteOffset : 입력 요소의 바이트 오프셋을 지정하는 정수입니다. 이 값은 입력 요소가 버퍼 내에서 어디에 위치하는지를 나타냅니다. D3D11_APPEND_ALIGNED_ELEMENT을 사용하면 이전 요소의 크기에 따라 자동으로 정렬됩니다.
	//D3D11_INPUT_CLASSIFICATION InputSlotClass : 입력 요소가 정점 데이터인지 인스턴스 데이터인지를 지정하는 열거형입니다. D3D11_INPUT_PER_VERTEX_DATA는 정점 데이터를 나타내고, D3D11_INPUT_PER_INSTANCE_DATA는 인스턴스 데이터를 나타냅니다.
	//UINT InstanceDataStepRate : 인스턴스 데이터의 경우, 각 인스턴스마다 데이터를 얼마나 자주 변경할지를 지정하는 정수입니다. 예를 들어, 1로 설정하면 각 인스턴스마다 데이터가 변경되고, 0으로 설정하면 모든 인스턴스가 동일한 데이터를 사용합니다.

	D3D11_INPUT_ELEMENT_DESC elementDesc = {};
	elementDesc.SemanticName = SemanticName;
	elementDesc.SemanticIndex = SemanticIndex;
	elementDesc.Format = Format;
	elementDesc.InputSlot = InputSlot;
	elementDesc.InputSlotClass = InputSlotClass;
	elementDesc.InstanceDataStepRate = InstanceDataStepRate;

	// 직접 계산 대신 GPU가 앞의 Format 크기를 보고 자동으로 오프셋을 설정
	elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	//elementDesc.AlignedByteOffset = mByteOffset;
	//mByteOffset += InputSize;

	mVecElementDescs.push_back(elementDesc);
}

bool CGraphicShader::createInputLayout(ID3D11Device& device, ID3DBlob* blob)
{
	HRESULT hr = device.CreateInputLayout(
		//&mVecElementDescs[0],
		mVecElementDescs.data(),
		static_cast<UINT>(mVecElementDescs.size()),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&mInputLayout);

	if (FAILED(hr))
		return false;

	return true;
}

bool CGraphicShader::compileShader(const std::wstring& fileName, const std::string& entryPoint, const std::string& shaderModel, ID3DBlob** shaderBlob)
{
	//TCHAR fullPath[MAX_PATH] = {};
	//lstrcpy(fullPath, mDefaultShaderFilePath.c_str());
	//lstrcat(fullPath, fileName.c_str());
	std::wstring fullPath = mDefaultShaderFilePath + fileName;

	UINT compileFlags = 0;
#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(
		fullPath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint.c_str(),
		shaderModel.c_str(),
		compileFlags,
		0,
		shaderBlob,
		&errorBlob);

	if (FAILED(hr))
	{
#ifdef _DEBUG
		if (errorBlob)
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
#endif // _DEBUG
		return false;
	}

	return true;
}

bool CGraphicShader::createVertexShader(ID3D11Device& device, const std::wstring& fileName, const std::string& entryPoint, const std::string& shaderModel)
{
	if(!compileShader(fileName, entryPoint, shaderModel, &mVertexShaderBlob))
		return false;

	if (FAILED((device.CreateVertexShader(
		mVertexShaderBlob->GetBufferPointer(),
		mVertexShaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader))))
		return false;

	return true;
}

bool CGraphicShader::createHullShader(ID3D11Device& device, const std::wstring& fileName, const std::string& entryPoint, const std::string& shaderModel)
{
	if (!compileShader(fileName, entryPoint, shaderModel, &mHullShaderBlob))
		return false;

	if (FAILED(device.CreateHullShader(
		mHullShaderBlob->GetBufferPointer(),
		mHullShaderBlob->GetBufferSize(),
		nullptr,
		&mHullShader)))
		return false;

	return true;
}

bool CGraphicShader::createDomainShader(ID3D11Device& device, const std::wstring& fileName, const std::string& entryPoint, const std::string& shaderModel)
{
	if (!compileShader(fileName, entryPoint, shaderModel, &mDomainShaderBlob))
		return false;

	if (FAILED(device.CreateDomainShader(
		mDomainShaderBlob->GetBufferPointer(),
		mDomainShaderBlob->GetBufferSize(),
		nullptr,
		&mDomainShader)))
		return false;

	return true;
}

bool CGraphicShader::createGeometryShader(ID3D11Device& device, const std::wstring& fileName, const std::string& entryPoint, const std::string& shaderModel)
{
	if (!compileShader(fileName, entryPoint, shaderModel, &mGeometryShaderBlob))
		return false;

	if (FAILED(device.CreateGeometryShader(
		mGeometryShaderBlob->GetBufferPointer(),
		mGeometryShaderBlob->GetBufferSize(),
		nullptr,
		&mGeometryShader)))
		return false;

	return true;
}

bool CGraphicShader::createPixelShader(ID3D11Device& device, const std::wstring& fileName, const std::string& entryPoint, const std::string& shaderModel)
{
	if (!compileShader(fileName, entryPoint, shaderModel, &mPixelShaderBlob))
		return false;

	if (FAILED(device.CreatePixelShader(
		mPixelShaderBlob->GetBufferPointer(),
		mPixelShaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader)))
		return false;

	return true;
}

