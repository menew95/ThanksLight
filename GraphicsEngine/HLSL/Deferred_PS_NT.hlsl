#pragma pack_matrix( row_major )
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

PixelOutput Deferred_PS_NT(PS_IN input)
{
	PixelOutput pixelOutput;

	pixelOutput.Albedo = float4(1.0f, 1.0f, 1.0f, material.metallic);
	pixelOutput.Normal = float4((input.Normal * 0.5 + 0.5), material.roughness);
	pixelOutput.Depth = float4(input.PosH.z, input.PosH.z, input.PosH.z, 1.0f);
	pixelOutput.WorldPos = float4(input.PosW, 1.0f);
	pixelOutput.Specular = float4(1.0f, 1.0f, 1.0f, input.PosV.z);

	return pixelOutput;
}