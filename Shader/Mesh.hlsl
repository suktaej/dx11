#include "Shared.hlsli"

struct VS_INPUT
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

VS_OUTPUT ColorMeshVS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    float4x4 wvp = mul(gWorld, gVP);
    
    float4 pos = float4(input.Position, 1.f);
    output.Position = mul(pos, wvp);
    output.Color = input.Color;
    return output;
}

PS_OUT_SINGLE ColorMeshPS(VS_OUTPUT input)
{
    PS_OUT_SINGLE output = (PS_OUT_SINGLE)0;

    output.Color = input.Color;
    return output;
}