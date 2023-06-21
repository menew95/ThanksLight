#pragma pack_matrix( row_major )

#include "Light_Header.hlsli"

#define MAX_PROJECTED_COUNT 5

Texture2D gWorldPos	: register(t0);
Texture2D ProjectedTexture	: register(t1);
Texture2D ProjectedDepth	: register(t2);

SamplerState samLinearClamp : register(s0);
SamplerComparisonState samShadow : register(s1);

struct PS_IN
{
	float4 PosH	   : SV_POSITION;
	float2 UV      : UV;
};

cbuffer PerObject_Projected: register(b2)
{
	matrix camViewProj;
}

float4 ProjectedPass_PS(PS_IN input) : SV_TARGET
{
	float4 worldPos = gWorldPos.Sample(samLinearClamp, input.UV);

	float4 projectedH = mul(float4(worldPos.xyz, 1.0f), camViewProj);
	projectedH.xyz /= projectedH.w;

	projectedH.x = projectedH.x * 0.5 + 0.5;
	projectedH.y = -projectedH.y * 0.5 + 0.5;

	float shadows = 1.0f;
	if (saturate(projectedH.x) == projectedH.x && saturate(projectedH.y) == projectedH.y)
	{
		float4 projectedColor = float4(0,0,0,0); 

		shadows = CalcShadowFactor2(samLinearClamp, ProjectedDepth, float3(projectedH.xyz));

		projectedColor += ProjectedTexture.Sample(samLinearClamp, projectedH.xy);

		clip(shadows < 1 ? -1 : 1);
		clip(projectedColor.a == 0 ? -1 : 1);
		projectedColor *= shadows;

		//clip(projectedColor.a == 0 ? -1 : 1);

		return projectedColor;
	}

	clip(-1);

	return float4(0, 0, 0, 0);
}