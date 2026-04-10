#pragma once
#include "../GameInfo.h"
#include "ConstantBufferInfo.h"

class CConstantBuffer
{
public:
	class ConstantBufferKey
	{
		friend class CShaderManager;
		ConstantBufferKey() {}
	};

	CConstantBuffer(ConstantBufferKey key);
	~CConstantBuffer();

private:
	ComPtr<ID3D11Buffer> mBuffer = nullptr;
	EShaderBufferType mBufferType = EShaderBufferType::None;
	int mSize = 0;
	int mRegisterSlot = 0;

public:
	bool init(EShaderBufferType bufferType, int size, int registerSlot);
	void updateBuffer(const void* data);
};
