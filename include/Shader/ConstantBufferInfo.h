#pragma once
#include "../GameInfo.h"

struct FTransformConstantBufferInfo
{
	XMFLOAT4 World;
	XMFLOAT4 View;
	XMFLOAT4 Projection;
	XMFLOAT4 WVP;
};
