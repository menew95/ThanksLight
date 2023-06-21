#pragma pack_matrix( row_major )

#include "ConstBuffer_Header.hlsli"

Texture2D gNormalMap : register(t0);
Texture2D gDepthMap : register(t1);
Texture2D gRandomVecMap : register(t2);

SamplerState samPointBorder : register(s0)
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU = BORDER;
	AddressV = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 1e5f);
};

SamplerState samPointWrap : register(s1)
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct PS_IN
{
	float4 PosH : SV_POSITION;
	float3 ToFarPlane : TEXCOORD0;
	float2 UV : UV;
};

float OcclusionFunction(float distZ)
{
	float occlusion = 0.0f;

	if (distZ > ssaoInfo.surfaceEpsilon)
	{
		float fadeLength = ssaoInfo.occlusionFadeEnd - ssaoInfo.occlusionFadeStart;

		occlusion = saturate((ssaoInfo.occlusionFadeEnd - distZ) / fadeLength);
	}

	return occlusion;
}

float NdcDepthToViewDepth(float2 UV, float Depth)
{
	float4 ndc = float4(UV * 2.0f - 1.0f, Depth, 1.0f);
	ndc.y *= -1.0f;
	float4 vp = mul(ndc, camera.projInv);
	float4 temp = (vp / vp.w);
	temp.x += 1.f;
	temp.y += 1.f;
	temp.z += 1.f;
	return (vp / vp.w).z;

	// z_ndc = A + B/viewZ, where gProj[2,2]=A and gProj[3,2]=B.
	//float viewZ = gProj[3][2] / (z_ndc - gProj[2][2]);
	//return viewZ;
}

float4 SSAO_PS(PS_IN input) : SV_TARGET
{
	float4 normal = gNormalMap.Sample(samPointBorder, input.UV);

	float4 depth = gDepthMap.Sample(samPointBorder, input.UV);

	normal = (normal - 0.5f) * 2.f;

	float3 n = normal.xyz;

	//float pz = depth.r;
	//float pz = NdcDepthToViewDepth(input.UV, depth.r);
	float pz = depth.w;

	float3 p = (pz / input.ToFarPlane.z) * input.ToFarPlane;

	//float3 randVec = randomNormal(input.uv);
	float3 randVec = 2.0f * gRandomVecMap.Sample(samPointWrap, 4.0f * input.UV).rgb - 1.0f;

	float occlusionSum = 0.0f;

	[unroll]
	for (int i = 0; i < 14; i++)
	{
		float3 offset = reflect(ssaoInfo.offSetVectors[i].xyz, randVec);

		float flip = sign(dot(offset, n));

		float3 q = p + flip * ssaoInfo.occlusionRadius * offset;

		float4 projQ = mul(float4(q, 1.0f), camera.viewToTexSpace);
		projQ /= projQ.w;

		projQ.x = projQ.x * 0.5f + 0.5;
		projQ.y = projQ.y * -0.5f + 0.5;

		//float rz2 = NdcDepthToViewDepth(gDepthMap.SampleLevel(samNormalDepth, projQ.xy, 0.0f).x);
		//float rz = gNormalMap.Sample(samNormalDepth, projQ.xy, 0.0f).a;
		float rz = gDepthMap.Sample(samPointBorder, projQ.xy).w;

		//rz = NdcDepthToViewDepth(projQ.xy, rz);

		float3 r = (rz / q.z) * q;

		float distZ = p.z - r.z;
		float dp = max(dot(n, normalize(r - p)), 0.0f);
		float occlusion = dp * OcclusionFunction(distZ);

		occlusionSum += occlusion;
	}

	occlusionSum /= 14;
	float access = 1.0f - occlusionSum;

	return saturate(pow(access, 4.0f));
}