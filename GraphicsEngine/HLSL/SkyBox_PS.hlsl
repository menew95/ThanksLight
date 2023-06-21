#pragma pack_matrix( row_major )

SamplerState samAnisotropicClamp : register(s0);

TextureCube gDiffuseMap : register(t0);

struct PS_IN
{
	float4 PosH	   : SV_POSITION;
	float3 WorldPos	   : POSITION;
};

// pixel shader
float4 SkyBox_PS(PS_IN input) : SV_Target
{
	return gDiffuseMap.Sample(samAnisotropicClamp, input.WorldPos);
}