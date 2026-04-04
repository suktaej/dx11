#pragma once
#include "../GameInfo.h"

struct FTransformConstantBufferInfo
{
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Projection;
	DirectX::XMFLOAT4X4 WVP;
};
