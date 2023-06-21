#pragma pack_matrix( row_major )

#include "Input_header.hlsli"
#include "ConstBuffer_Header.hlsli"

struct VS_OUT
{
	float4 PosH		: SV_POSITION;
	float4 Color	: COLOR;
};

VS_OUT OutLineMesh_VS(MeshInput input)
{
	VS_OUT output;

	output.PosH = mul(float4(input.PosL, 1.0f), worldViewProj);
	output.Color = input.Color;

	return output;
}