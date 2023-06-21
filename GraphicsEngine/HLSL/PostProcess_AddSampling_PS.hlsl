#pragma pack_matrix( row_major )

SamplerState samLinearClamp : register(s0);
Texture2D gDownMap : register(t0);
Texture2D gUpMap : register(t1);

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 UV : UV;
};

float4 PostProcess_AddSampling_PS(PS_IN input) : SV_Target0
{
    float3 _finColor = float3(0,0,0);

    _finColor += gDownMap.Sample(samLinearClamp, input.UV).rgb;
    _finColor += gUpMap.Sample(samLinearClamp, input.UV).rgb;

    return float4(_finColor.rgb, 1);
}