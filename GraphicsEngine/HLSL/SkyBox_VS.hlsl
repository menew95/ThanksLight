#pragma pack_matrix( row_major )

#include "ConstBuffer_Header.hlsli"

struct VS_IN
{
	float3 PosL		: POSITION;
};

struct VS_OUT
{
	float4 PosH		: SV_POSITION;
	float3 WorldPos	: POSITION;
};

// vertex shader 
VS_OUT SkyBox_VS(VS_IN input)
{
	VS_OUT output;

	output.PosH = mul(float4(input.PosL, 1.0f), camera.camWorldViewProj).xyzw;
	output.WorldPos = input.PosL;

	return output;
}