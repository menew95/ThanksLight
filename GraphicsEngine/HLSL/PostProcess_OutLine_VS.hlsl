#pragma pack_matrix( row_major )
#include "Output_Header.hlsli"
#include "ConstBuffer_Header.hlsli"

struct VS_IN
{
	float3 PosL : POSITION;
	float3 Normal : NORMAL;
	float2 UV : UV;
};

struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float2 UV : TEXCOORD0;
	float3 ScreenPos : TEXCOORD2;
};

float2 TransformTriangleVertexToUV(float2 vertex)
{
	float2 uv = (vertex + 1.0) * 0.5;
	return uv;
}

float4 ComputeScreenPos(float4 pos)
{
	float4 o = pos * 0.5f;
	o.xy = float2(o.x, o.y * projectionParams.x) + o.w;
	o.zw = pos.zw;
	return o;
}

// vertex shader 
VS_OUT PostProcess_OutLine_VS(VS_IN input)
{
	VS_OUT vout;

	vout.PosH = float4(input.PosL, 1.f);
	vout.UV = input.UV;// TransformTriangleVertexToUV();
	vout.ScreenPos = ComputeScreenPos(float4(input.UV, 0, 1));

	 return vout;
}