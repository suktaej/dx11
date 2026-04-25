#include "Shared.hlsli"

struct InstanceData
{
    matrix WorldMatrix;
};

// InstanceData 구조체가 일정 간격(stride)으로 나열된 GPU 버퍼를 셰이더에서 배열처럼 접근
StructuredBuffer<InstanceData> gInstanceBuffer : register(t0);

struct VS_INPUT
{
    float3 Position : POSITION;
    float4 Color : COLOR;
    uint InstanceID : SV_InstanceID; // 인스턴싱 정보
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

VS_OUTPUT ColorMeshVS(VS_INPUT input)
{
    // 구조체 멤버 초기화
    VS_OUTPUT output = (VS_OUTPUT) 0;

    // 지역공간에서 월드공간으로 변환
    // 아핀공간에서 변환할 때는 w값이 1(점)
    
    matrix world = gInstanceBuffer[input.InstanceID].WorldMatrix;
    
    float4 pos = float4(input.Position, 1.f);
    pos = mul(pos, world);
    pos = mul(pos, gVP);
    output.Position = pos;
    
    output.Color = input.Color;

    return output;
}

// 필요한 정보(Color)만 전달받을 수 있도록 Semantic을 사용해 값을 추출
// Register 번호가 없을 경우 0번 레지스터
// PS_OUT_SINGLE ColorMeshPS(float4 Color : COLOR)
PS_OUT_SINGLE ColorMeshPS(VS_OUTPUT input)
{
    PS_OUT_SINGLE output = (PS_OUT_SINGLE) 0;

    output.Color = input.Color;
    return output;
}