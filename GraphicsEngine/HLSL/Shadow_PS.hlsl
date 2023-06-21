#pragma pack_matrix( row_major )

#include "Input_Header.hlsli"

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex  : TEXCOORD;
};

float4 Shadow_PS(VertexOut pin) : SV_TARGET
{
	return float4(pin.PosH.z,pin.PosH.z,pin.PosH.z, 1.0f);
}