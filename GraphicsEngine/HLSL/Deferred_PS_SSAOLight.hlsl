#pragma pack_matrix( row_major )

Texture2D gAlbedo	: register(t0);
Texture2D gNormal	: register(t1);
Texture2D gDepth	: register(t2);
Texture2D gWorldPos	: register(t3);
Texture2D gSpec		: register(t4);
Texture2D gSSAOMap	: register(t5);

SamplerState samLinearWrap : register(s0);

struct DirectionalLight
{	
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};
	
cbuffer cbPerFrame : register(b0)
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;
};

struct PS_IN
{
	float4 PosH	   : SV_POSITION;
	float2 UV      : UV;
};

// pixel shader
float4 Deferred_PS_SSAOLight(PS_IN input) : SV_TARGET
{
	float4 depth = gDepth.Sample(samLinearWrap, input.UV);

	clip(depth.x == 1.0f ? -1 : 1);

	float4 albedo = gAlbedo.Sample(samLinearWrap, input.UV);
	float4 normal = gNormal.Sample(samLinearWrap, input.UV);
	float4 wordlPos = gWorldPos.Sample(samLinearWrap, input.UV);
	float4 specular = gSpec.Sample(samLinearWrap, input.UV);

	normal.a = 1.0f;
	normal = (normal - 0.5f) * 2.f;
	albedo = pow(albedo, 2.2);

	//////Light

	float4 ambient = float4(depth.z, depth.z, depth.z, 1.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 emis = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -gDirLights[0].Direction;

	float3 toEye = normalize(gEyePosW - wordlPos.xyz);

	float NdotL = dot(lightVec, normal.xyz);

	// 전체광의 ambient를 기준으로 자신의 ambient색을 곱해줌
	ambient *= gDirLights[0].Ambient;

	float ambientAccess = gSSAOMap.Sample(samLinearWrap, input.UV).r;

	ambient *= ambientAccess;

	[flatten]
	if (NdotL > 0.0f)
	{
		diffuse = NdotL * float4(depth.y, depth.y, depth.y, 1.0f) * gDirLights[0].Diffuse;

		if (any(specular.rgb))
		{
			// 반사 벡터, direction과 노말을 dot
			// 진입각으로 만들기 위해서 다시 뒤접이서 계산필요
			float3 R = normalize(reflect(-lightVec, normal.xyz));

			float4 RdotE = saturate(dot(R, toEye));

			float4 specFactor = pow(RdotE, 32.0f);

			spec = specFactor * specular * gDirLights[0].Specular;
		}
	}

	float4 litColor = albedo * float4(((ambient + diffuse).rgb), 1.0f) + spec;

	litColor = pow(litColor, 1.0f / 2.2f);

	litColor.w = 1.0f;

	return saturate(litColor);
}