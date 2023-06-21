#pragma pack_matrix( row_major )

#include "ConstBuffer_Header.hlsli"

struct VS_IN
{
	float3 PosL : POSITION;
	float3 ToFarPlaneIndex : NORMAL;
	float2 UV : UV;
};

struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float3 ToFarPlane : TEXCOORD0;
	float2 UV : UV;
};


VS_OUT SSAO_VS(VS_IN input)
{
	VS_OUT output;

	output.PosH = float4(input.PosL, 1.f);
	output.ToFarPlane = ssaoInfo.frustumCorners[input.ToFarPlaneIndex.x].xyz;
	output.UV = input.UV;

	return output;
}