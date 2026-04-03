#include "ConstantBuffer.h"
#include "../DeviceManager.h"

CConstantBuffer::CConstantBuffer()
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

	mDeviceMgr->getContext()->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

	std::copy(static_cast<const char*>(data), static_cast<const char*>(data) + mSize, static_cast<char*>(Map.pData));

	mDeviceMgr->getContext()->Unmap(mBuffer.Get(), 0);
}
