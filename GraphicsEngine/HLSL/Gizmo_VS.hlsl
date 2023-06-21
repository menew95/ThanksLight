#pragma pack_matrix( row_major )

#include "ConstBuffer_Header.hlsli"

struct VS_IN
{
    float3 PosL		: POSITION;
    float4 Color	: COLOR;
};

struct VS_OUT
{
    float4 PosH	   : SV_POSITION;
    float4 Color   : COLOR;
};

VS_OUT Gizmo_VS(VS_IN vin)
{
    VS_OUT vout;

    vout.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);
    vout.Color = vin.Color;

    return vout;
}