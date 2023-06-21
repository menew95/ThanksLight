#pragma pack_matrix( row_major )

#include "Math_Header.hlsli"
#include "ConstBuffer_Header.hlsli"

struct PS_IN
{
	float4 PosH		: SV_POSITION;
	float4 Color	: COLOR;
	float2 UV		: UV;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;

	float3 PosV		: POSITION1;
	float3 PosW		: POSITION2;
};

struct PixelOutput
{
	float4 Albedo	: SV_Target0;
	float4 Normal	: SV_Target1;
	float4 Depth	: SV_Target2;
	float4 WorldPos	: SV_Target3;
	float4 Specular	: SV_Target4;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap	: register(t0);
Texture2D gNormalMap	: register(t1);
Texture2D gMRAOMap		: register(t2);

SamplerState samAnisotropicWrap : register(s0);

PixelOutput Deferred_PS_PBR(PS_IN input)
{
	PixelOutput pixelOutput;

	// 텍스처에서 표본을 추출한다
	float4 baseColor = gDiffuseMap.Sample(samAnisotropicWrap, input.UV);

	float3 normalMapSample = gNormalMap.Sample(samAnisotropicWrap, input.UV).rgb;
	float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, input.Normal, input.Tangent);
	float3 metallicRoughnessAOMap = gMRAOMap.Sample(samAnisotropicWrap, input.UV).rgb;
	
	baseColor.w = 1.0f;
	bumpedNormalW -= bumpedNormalW;
	bumpedNormalW = input.Normal;
	pixelOutput.Albedo = baseColor;
	pixelOutput.Normal = float4((bumpedNormalW * 0.5 + 0.5), metallicRoughnessAOMap.b);
	pixelOutput.Depth = float4(input.PosH.z, input.PosH.z, input.PosH.z, 1.0f);
	pixelOutput.WorldPos = float4(input.PosW, 1.0f);
	pixelOutput.Specular = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return pixelOutput;
}