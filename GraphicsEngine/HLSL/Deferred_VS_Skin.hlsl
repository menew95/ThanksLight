#pragma pack_matrix( row_major )

#include "Input_Header.hlsli"
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

VS_OUT Deferred_VS_Skin(SkinMeshInput input)
{
	VS_OUT output;

	// Transform to homogeneous clip space.
	float weight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weight[0] = input.Weight.x;
	weight[1] = input.Weight.y;
	weight[2] = input.Weight.z;
	weight[3] = 1.0f - weight[0] - weight[1] - weight[2];

	float3 posL = { 0.0f, 0.0f, 0.0f };
	float3 normalL = { 0.0f, 0.0f, 0.0f };
	float3 tangentL = { 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < 3; i++)
	{
		posL += weight[i] * mul(float4(input.PosL, 1.0f), boneTransforms[input.Bone[i]]).xyz;
		normalL += weight[i] * mul(input.Normal, (float3x3)boneTransforms[input.Bone[i]]).xyz;
		tangentL += weight[i] * mul(input.Tangent, (float3x3)boneTransforms[input.Bone[i]]).xyz;
	}

	output.PosH = mul(float4(posL, 1.0f), worldViewProj);
	output.Color = input.Color;
	output.UV = input.UV;
	output.Normal = normalize(mul(normalL, (float3x3)worldInvTranspose));
	output.Tangent = normalize(mul(tangentL, (float3x3)world));
	output.PosV = mul(float4(posL, 1.0f), worldView).xyz;
	output.PosW = mul(float4(posL, 1.0f), world).xyz;

	return output;
}