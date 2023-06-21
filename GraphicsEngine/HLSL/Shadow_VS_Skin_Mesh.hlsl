#pragma pack_matrix( row_major )

#include "Input_Header.hlsli"
#include "ConstBuffer_Header.hlsli"

float4 Shadow_VS_Skin_Mesh(SkinMeshInput input) : SV_POSITION
{
	// Transform to homogeneous clip space.
	float weight[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weight[0] = input.Weight.x;
	weight[1] = input.Weight.y;
	weight[2] = input.Weight.z;
	weight[3] = 1.0f - weight[0] - weight[1] - weight[2];

	float3 posL = { 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < 3; i++)
	{
		posL += weight[i] * mul(float4(input.PosL, 1.0f), boneTransforms[input.Bone[i]]).xyz;
	}

	return mul(float4(posL, 1.0f), worldViewProj);
}