#pragma pack_matrix( row_major )

#include "Input_Header.hlsli"
#include "ConstBuffer_Header.hlsli"

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex  : TEXCOORD;
};

VertexOut Shadow_VS_Static_Mesh(MeshInput vin)
{
	VertexOut output;
	output.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);
	output.Tex = float2(0,0);
	return output;
}