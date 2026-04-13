#pragma once
#include "../GameInfo.h"

//struct FTransformConstantBufferInfo
//{
//	DirectX::XMFLOAT4X4 World;
//	DirectX::XMFLOAT4X4 View;
//	DirectX::XMFLOAT4X4 Projection;
//	DirectX::XMFLOAT4X4 WVP;
//};

// 프레임당 한 번만 바뀌는 데이터
// 모든 오브젝트가 공유
struct FFrameConstantBufferInfo 
{
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Projection;
	DirectX::XMFLOAT4X4 VP;
};

// 오브젝트마다 바뀌는 데이터
struct FObjectConstantBufferInfo 
{
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 WVP;	// vertex shader에서 즉시 사용
	//DirectX::XMFLOAT4X4 WorldInvTranspose; // 노멀(Normal) 계산용
};
