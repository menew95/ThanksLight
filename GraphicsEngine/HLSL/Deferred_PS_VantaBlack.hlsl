#pragma pack_matrix( row_major )
#include "Output_Header.hlsli"
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

// ÁË¾ÇÀÌ¿ä ¸ðµç °ÍÀ» Áû¾îÁø ÁË¾ÇÀÌ¿ä
cbuffer cbVantaObject : register(b3)
{
	float3 Direction;	// Directional
	float SpotAngle;
	int isCamLight;
	int intPad[3];
};

Deferred_PSOut Deferred_PS_VantaBlack(PS_IN input)
{
	Deferred_PSOut pixelOutput;

	pixelOutput.Albedo = float4(1.0f, 1.0f, 1.0f, material.metallic);
	pixelOutput.Normal = float4((input.Normal * 0.5 + 0.5), material.roughness);
	pixelOutput.Depth = float4(input.PosH.z, input.PosH.z, input.PosH.z, 1.0f);


	if (isCamLight > 0)
	{
		float3 dir = normalize(input.PosW.xyz - camera.cameraPosition.xyz);
		float degree = dot(Direction, dir);
		float att = 1 - 0.86602540378f; // cos 30µµ
		if (degree > SpotAngle)
		{
			pixelOutput.WorldPos = float4(input.PosW, 1 - (1 - degree) / att);
		}
		else
		{
			pixelOutput.WorldPos = float4(input.PosW, 0.0f);
		}
	}
	else
	{
		pixelOutput.WorldPos = float4(input.PosW, 0.0f);
	}

	pixelOutput.Emissive = float4(0.0f, 0.0f, 0.0f, 0.0f);

	return pixelOutput;
}