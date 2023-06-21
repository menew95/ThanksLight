#pragma pack_matrix( row_major )

#include "Input_header.hlsli"
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

VS_OUT Deferred_VS_Mesh(MeshInput input)
{
	VS_OUT output;

	output.PosH = mul(float4(input.PosL, 1.0f), worldViewProj);
	output.Color = input.Color;
	output.UV = input.UV;
	output.Normal = normalize(mul(input.Normal, (float3x3)worldInvTranspose));
	output.Tangent = normalize(mul(input.Tangent, (float3x3)world));
	output.PosV = mul(float4(input.PosL, 1.0f), worldView).xyz;
	output.PosW = mul(float4(input.PosL, 1.0f), world).xyz;

	return output;
}