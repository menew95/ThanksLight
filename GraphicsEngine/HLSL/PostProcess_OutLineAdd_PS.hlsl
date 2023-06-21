#pragma pack_matrix( row_major )
#include "Output_Header.hlsli"
#include "ConstBuffer_Header.hlsli"

Texture2D mainTex : register(t0);
Texture2D outLineTex : register(t1);

SamplerState samLinearClamp : register(s0);

struct PS_IN
{
	float4 PosH : SV_POSITION;
	float2 UV : UV;
};

float4 PostProcess_OutLineAdd_PS(PS_IN input) : SV_TARGET
{
	float4 original = mainTex.Sample(samLinearClamp, input.UV);
	float4 outline = outLineTex.Sample(samLinearClamp, input.UV);

	float4 output = lerp(original, outline, 1);
	output = outline + original;
	return output;
}