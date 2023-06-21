#pragma pack_matrix( row_major )

#include "ConstBuffer_Header.hlsli"

//#define BLOOMCURVE_METHOD_1
//#define BLOOMCURVE_METHOD_2
#define BLOOMCURVE_METHOD_3

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap : register(t0);

SamplerState samLinearClamp : register(s0);


struct PS_IN
{
	float4 PosH : SV_POSITION;
	float2 UV : UV;
};

float GetBloomCurve(float x, float threshold)
{
	float result = x;
	x *= 2.0f;

#ifdef BLOOMCURVE_METHOD_1
	result = x * 0.05 + max(0, x - threshold) * 0.5; // default threshold = 1.26
#endif

#ifdef BLOOMCURVE_METHOD_2
	result = x * x / 3.2;
#endif

#ifdef BLOOMCURVE_METHOD_3
	result = max(0, x - threshold); // default threshold = 1.0
	result *= result;
#endif

	return result * 0.5f;
}

float4 PostProcess_Bloom_PS(PS_IN input) : SV_TARGET
{
    float3 outColor = float3(0.0f, 0.0f, 0.0f);

    [unroll]
    for (uint y = 0; y < 4; y++)
    {
        [unroll]
        for (uint x = 0; x < 4; x++)
        {
            // Compute the sum of color values
            outColor += gDiffuseMap.Sample(samLinearClamp, input.UV, int2(x, y)).rgb;
        }
    }

    outColor /= 16;

    float intensity = max(dot(outColor, float3(0.3f, 0.3f, 0.3f)), 0.000001f);

    float bloom_intensity = min(GetBloomCurve(intensity, threshold_Min), threshold_Max);
    float3 bloom_color = outColor * bloom_intensity / intensity;

    return float4(bloom_color, 1.0f);
}