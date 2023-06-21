#pragma pack_matrix( row_major )
#include "Math_Header.hlsli"
#include "ConstBuffer_Header.hlsli"

struct VS_OUT
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
	float4 Emissive	: SV_Target4;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);

SamplerState samAnisotropicWrap : register(s0);
SamplerState samLinearMirror : register(s1);

PixelOutput Deferred_PS(VS_OUT input)
{
	PixelOutput pixelOutput;
	// 기본 텍스처 색상은 곱셈의 항등원
	float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	texColor *= material.diffuse;

	if (material.isUseAlbedoTex)
	{
		float2 tileUV = input.UV * material.tiling;
		texColor *= gDiffuseMap.Sample(samLinearMirror, tileUV);
	}

	texColor.a = material.metallic;

	float3 normal = input.Normal.xyz;

	if (material.isUseBumpTex)
	{
		float2 tileUV = input.UV * material.tiling;
		float3 normalMapSample = gNormalMap.Sample(samLinearMirror, tileUV).rgb;
		float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, input.Normal, input.Tangent);

		normal = bumpedNormalW;
	}

	pixelOutput.Albedo = texColor;
	pixelOutput.Normal = float4((normal * 0.5 + 0.5), material.roughness);
	pixelOutput.Depth = float4(input.PosH.z, input.PosH.z, input.PosH.z, 1.0f);
	pixelOutput.WorldPos = float4(input.PosW, 1.0f);
	pixelOutput.Emissive = float4(float3(material.emissive.rgb), input.PosV.z);

	return pixelOutput;
}