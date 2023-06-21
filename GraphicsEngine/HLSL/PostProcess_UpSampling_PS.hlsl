#pragma pack_matrix( row_major )

cbuffer cbUpSampling : register(b0)
{
    // 0 => x, 1 => y
    float4 UpInfo;
};

SamplerState samPointWrap : register(s0);
Texture2D gDownMap : register(t0);
Texture2D gUpMap : register(t1);

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 UV : UV;
};

//--------------------------------------------------
// Pixel Shader - Simple Blur
//--------------------------------------------------
float4 PostProcess_UpSampling_PS(PS_IN input) : SV_Target0
{
    float3 _finColor = float3(0,0,0);

    _finColor += gDownMap.Sample(samPointWrap, input.UV).rgb * UpInfo.x;
    _finColor += gUpMap.Sample(samPointWrap, input.UV).rgb * UpInfo.y;

    return float4(_finColor.rgb, 1);
}