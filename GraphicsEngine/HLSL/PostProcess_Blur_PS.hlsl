#pragma pack_matrix( row_major )

#include "ConstBuffer_Header.hlsli"

#define POSTEFFECT_BLUR_SAMPLING_COUNT 9

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap : register(t0);

SamplerState samLinearClamp : register(s0);

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 UV : UV;
};

//--------------------------------------------------
// Pixel Shader - Simple Blur
//--------------------------------------------------
float4 PostProcess_Blur_PS(PS_IN input) : SV_Target0
{
    float weights[POSTEFFECT_BLUR_SAMPLING_COUNT] =
    {
        0.013519569015984728,
        0.047662179108871855,
        0.11723004402070096,
        0.20116755999375591,
        0.240841295721373,
        0.20116755999375591,
        0.11723004402070096,
        0.047662179108871855,
        0.013519569015984728
    };

    float indices[POSTEFFECT_BLUR_SAMPLING_COUNT] = { -4, -3, -2, -1, 0, +1, +2, +3, +4 };

    float2 dir;
    if (blurInfo.x > 0)
    {
        dir = float2(0.0, 1.0);
    }
    else
    {
        dir = float2(1.0, 0.0);
    }

    float2 step = float2(dir.x * blurInfo.z, dir.y * blurInfo.w);
    float4 Result = 0.0f;

    for (int i = 0; i < POSTEFFECT_BLUR_SAMPLING_COUNT; i++)
    {
        Result += gDiffuseMap.Sample(samLinearClamp, float2(input.UV + indices[i] * step)) * weights[i];
    }

    return float4(Result.rgb, 1);

}