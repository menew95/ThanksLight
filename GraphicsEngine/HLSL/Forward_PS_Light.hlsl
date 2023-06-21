#pragma pack_matrix( row_major )
#include "Math_Header.hlsli"
#include "ConstBuffer_Header.hlsli"

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap	: register(t0);
Texture2D gNormalMap	: register(t1);

#ifdef MRAMap
Texture2D gMRAOMap		: register(t2);
#endif

// IBL
Texture2D gBRDFMap	: register(t3);
TextureCube gIrradianceMap	: register(t4);
TextureCube gEnvMap	: register(t5);

Texture2D gShadow[MAX_LIGHT_COUNT]	: register(t6);

SamplerState samLinearWrap : register(s0);
SamplerState samLinearClamp : register(s1);
SamplerState samAnisotropicWrap : register(s2);
SamplerComparisonState samShadow : register(s3);

struct Foward_VS_OUT
{
	float4 PosH		: SV_POSITION;
	float4 Color	: COLOR;
	float2 UV		: UV;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;

	float3 PosV		: POSITION1;
	float3 PosW		: POSITION2;
};

float3 FresnelSchlickRoughness(float NoV, float3 specColor, float roughness)
{
	float roughnessPercent = 1.0f - roughness;
	return specColor + (max(roughnessPercent, specColor) - specColor) * pow(1.0 - NoV, 5.0f);
}

float3 IBL(float3 albedo, float roughness, float metallic, float3 N, float3 V)
{
	float NoV = saturate(dot(N, V));
	float3 R = 2 * dot(V, N) * N - V;

	float3 irradiance = gIrradianceMap.Sample(samLinearWrap, N).rgb;
	float3 preFilteredEnv = gEnvMap.SampleLevel(samLinearWrap, R, roughness * 10).rgb;
	float2 brdf = gBRDFMap.Sample(samAnisotropicWrap, float2(max(dot(N, V), 0.0f), roughness)).rg;

	float3 F0 = lerp(0.04f, albedo, metallic);

	float3 kS = FresnelSchlickRoughness(max(dot(N, V), 0.0f), F0, roughness);
	float3 specular = preFilteredEnv * (kS * brdf.r + brdf.g);

	float3 kD = (1 - kS) * (1 - metallic);

	float3 diffuse = albedo * irradiance * kD;

	return diffuse + specular;
}

float4 Forward_PS_Light(Foward_VS_OUT input) : SV_TARGET
{
	float4 emissive = pow(material.emissive, 2.2f);

	float3 _finColor = emissive.rgb;

	float4 shadowH = float4(0., 0., 0., 0.);

	float shadows = 1.0f;

	float roughness = lerp(0.04f, 1.0f, material.roughness);
	float metallic = lerp(0.04f, 1.0f, material.metallic);

	float3 albedo = float3(1, 1, 1);

	albedo *= material.diffuse;

	if (material.isUseAlbedoTex)
	{
		albedo *= gDiffuseMap.Sample(samAnisotropicWrap, input.UV);
	}

	albedo = pow(albedo, 2.2);

	float3 normal = input.Normal;


	if (material.isUseBumpTex)
	{
		float3 normalMapSample = gNormalMap.Sample(samAnisotropicWrap, input.UV).rgb;
		normal = NormalSampleToWorldSpace(normalMapSample, input.Normal, input.Tangent);
	}

#ifdef MRAMap
	float3 metallicRoughnessAOMap = gMRAOMap.Sample(samAnisotropicWrap, input.UV).rgb;
	roughness *= metallicRoughnessAOMap.b;
	metallic *= metallicRoughnessAOMap.r;
#endif

	float3 toEye = normalize(camera.cameraPosition - input.PosW);

	float roughness2 = max(.001, roughness * roughness);

	float3 specularColor = albedo * metallic;
	float3 diffuseColor = albedo * (1 - metallic);

	[unroll]
	for (int i = 0; i < LightCount; i++)
	{
		if (LightInfo[i].Type != 1)
		{
			shadowH = mul(float4(input.PosW, 1.0f), LightInfo[i].ShadowTransform);
			shadowH.xyz /= shadowH.w;

			shadows = CalcShadowFactor(samShadow, gShadow[i], float3(shadowH.xyz));
		}

		_finColor += CalLight(LightInfo[i], specularColor, diffuseColor, input.PosW, normal, toEye, roughness2, metallic) * shadows;
	}

	float3 ibl = IBL(albedo.rgb, roughness, metallic, normal, toEye) /* shadows*/;

	_finColor += ibl * IBLFactor;
	_finColor = pow(_finColor, 1 / 2.2);


	return float4(_finColor, 1.0f);
}