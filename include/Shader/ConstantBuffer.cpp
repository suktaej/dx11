#include "ConstantBuffer.h"
#include "../DeviceManager.h"

CConstantBuffer::CConstantBuffer(ConstantBufferKey key)
{
}

CConstantBuffer::~CConstantBuffer()
{
}

bool CConstantBuffer::init(CDeviceManager* device, EShaderBufferType bufferType, int size, int registerSlot)
{
	mDeviceMgr = device;

	mSize = size;
	mRegisterSlot = registerSlot;
	mBufferType = bufferType;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = size;
	// WVP 정보는 매 프레임마다 업데이트되어야 하므로 D3D11_USAGE_DYNAMIC으로 생성
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// CPU에서 Constant Buffer에 데이터를 쓰기 위해 D3D11_CPU_ACCESS_WRITE 플래그 설정
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = device->getDevice()->CreateBuffer(&bufferDesc, nullptr, &mBuffer);

	if (FAILED(hr))
		return false;

	return true;
}

void CConstantBuffer::updateBuffer(const void* data)
{
	// Buffer 안에 있는 데이터를 저장하기 위한 배열의 주소를 받아올 구조체 선언
	D3D11_MAPPED_SUBRESOURCE Map = {};
	// GPU에서 Buffer에 접근하기 위해 Map 메서드 호출
	mDeviceMgr->getContext()->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);
	// Map.pData는 Buffer에 접근하기 위한 포인터이므로, data에서 mSize만큼의 데이터를 Map.pData로 복사
	std::copy(static_cast<const char*>(data), static_cast<const char*>(data) + mSize, static_cast<char*>(Map.pData));
	// Buffer에 대한 접근이 끝났으므로 Unmap 메서드 호출
	mDeviceMgr->getContext()->Unmap(mBuffer.Get(), 0);

	// Shader를 지정하여 Constant Buffer의 data를 binding
	// SetConstantBuffers는 ConstantBuffer를 double pointer로 받아야 하므로
	// mBuffer.GetAddressOf()를 사용하여 double pointer로 전달
	// 만일 다수의 ConstantBuffer를 한 번에 전달해야 하는 경우, 배열을 만들어 전달해야 함
	if((mBufferType & EShaderBufferType::Vertex) != EShaderBufferType::None)
		mDeviceMgr->getContext()->VSSetConstantBuffers(mRegisterSlot, 1, mBuffer.GetAddressOf());

	if((mBufferType & EShaderBufferType::Pixel) != EShaderBufferType::None)
		mDeviceMgr->getContext()->PSSetConstantBuffers(mRegisterSlot, 1, mBuffer.GetAddressOf());

	if((mBufferType & EShaderBufferType::Hull) != EShaderBufferType::None)
		mDeviceMgr->getContext()->HSSetConstantBuffers(mRegisterSlot, 1, mBuffer.GetAddressOf());

	if((mBufferType & EShaderBufferType::Domain) != EShaderBufferType::None)
		mDeviceMgr->getContext()->DSSetConstantBuffers(mRegisterSlot, 1, mBuffer.GetAddressOf());

	if ((mBufferType & EShaderBufferType::Geometry) != EShaderBufferType::None)
		mDeviceMgr->getContext()->GSSetConstantBuffers(mRegisterSlot, 1, mBuffer.GetAddressOf());

	if((mBufferType & EShaderBufferType::Compute) != EShaderBufferType::None)
		mDeviceMgr->getContext()->CSSetConstantBuffers(mRegisterSlot, 1, mBuffer.GetAddressOf());
}
