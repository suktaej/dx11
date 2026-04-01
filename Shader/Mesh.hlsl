#include "Shared.hlsli"

struct VS_INPUT_COLOR
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct VS_OUTPUT_COLOR
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

VS_OUTPUT_COLOR ColorMeshVS(VS_INPUT_COLOR input)
{
    // 구조체 멤버 초기화
    VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR)0;

    output.Position = float4(input.Position, 1.f);
    output.Color = input.Color;

    return output;
}

// 필요한 정보(Color)만 전달받을 수 있도록 Semantic을 사용해 값을 추출
// Register 번호가 없을 경우 0번 레지스터
// PS_OUT_SINGLE ColorMeshPS(float4 Color : COLOR)
PS_OUT_SINGLE ColorMeshPS(VS_OUTPUT_COLOR input)
{
    PS_OUT_SINGLE output = (PS_OUT_SINGLE)0;

    output.Color = input.Color;
    return output;
}