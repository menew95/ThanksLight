#pragma pack_matrix( row_major )

Texture2D gDiffuseMap : register(t0);

SamplerState samLinearClamp : register(s0);

struct PS_IN
{
	float4 PosH	   : SV_POSITION;
	float2 UV      : UV;
};

// pixel shader
float4 PostProcess_DownSampling_PS(PS_IN input) : SV_TARGET0
{
	return gDiffuseMap.Sample(samLinearClamp, input.UV);
}