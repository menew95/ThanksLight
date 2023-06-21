#pragma pack_matrix( row_major )
#include "Output_Header.hlsli"
#include "ConstBuffer_Header.hlsli"

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD0;
    float3 ScreenPos : TEXCOORD2;
};

Texture2D mainTex : register(t0);
Texture2D depthTex : register(t1);

SamplerState samLinearClamp : register(s0);

float Linear01Depth(float z)
{
    float isOrtho = orthoParams.w;
    float isPers = 1.0 - orthoParams.w;
    z *= zBufferParams.x;
    return (1.0 - isOrtho * z) / (isPers * z + zBufferParams.y);
}

float4 PostProcess_OutLine_PS(PS_IN input) : SV_TARGET
{
    float4 original = mainTex.Sample(samLinearClamp, input.UV);

    float4 depth = depthTex.Sample(samLinearClamp, input.UV);

    float offset_positive = +ceil(thickNess * 0.5f);
    float offset_negative = -floor(thickNess * 0.5f);
    float left = mainTex_TexelSize.x * offset_negative;
    float right = mainTex_TexelSize.x * offset_positive;
    float top = mainTex_TexelSize.y * offset_negative;
    float bottom = mainTex_TexelSize.y * offset_positive;

    float2 uv0 = input.UV + float2(left, top);
    float2 uv1 = input.UV + float2(right, bottom);
    float2 uv2 = input.UV + float2(right, top);
    float2 uv3 = input.UV + float2(left, bottom);

    float d0 = Linear01Depth(depthTex.Sample(samLinearClamp, uv0).r);
    float d1 = Linear01Depth(depthTex.Sample(samLinearClamp, uv1).r);
    float d2 = Linear01Depth(depthTex.Sample(samLinearClamp, uv2).r);
    float d3 = Linear01Depth(depthTex.Sample(samLinearClamp, uv3).r);

    float d = length(float2(d1 - d0, d3 - d2));
    d = smoothstep(minDepth, maxDepth, d);

    half output = d;

    return saturate(output);
}