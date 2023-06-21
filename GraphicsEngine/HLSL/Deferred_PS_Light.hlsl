#pragma pack_matrix( row_major )

#include "ConstBuffer_Header.hlsli"

Texture2D gAlbedo	: register(t0);
Texture2D gNormal	: register(t1);
Texture2D gDepth	: register(t2);
Texture2D gWorldPos	: register(t3);
Texture2D gEmissive	: register(t4);

// IBL
Texture2D gBRDFMap	: register(t5);
TextureCube gIrradianceMap	: register(t6);
TextureCube gEnvMap	: register(t7);

Texture2D gShadow[MAX_LIGHT_COUNT]	: register(t8);

SamplerState samLinearWrap : register(s0);
SamplerState samLinearClamp : register(s1);
SamplerState samAnisotropicWrap : register(s2);
SamplerComparisonState samShadow : register(s3);

struct PS_IN
{
	float4 PosH	   : SV_POSITION;
	float2 UV      : UV;
};

#define uSamplesCount 1024
#define PI 3.14159265359f

float3 GGX_Sample(float2 E, float alpha) {
	float Phi = 2.0 * PI * E.x;
	float cosThetha = saturate(sqrt((1.0 - E.y) / (1.0 + alpha * alpha * E.y - E.y)));
	float sinThetha = sqrt(1.0 - cosThetha * cosThetha);
	return float3(sinThetha * cos(Phi), sinThetha * sin(Phi), cosThetha);
}

float3x3 GetSampleTransform(float3 Normal) {
	float3x3 w;
	float3 up = abs(Normal.y) < 0.999 ? float3(0, 1, 0) : float3(1, 0, 0);
	w[0] = normalize(cross(up, Normal));
	w[1] = cross(Normal, w[0]);
	w[2] = Normal;
	return w;
}

float2 Hammersley(float i, float numSamples)
{
	uint b = uint(i);

	b = (b << 16u) | (b >> 16u);
	b = ((b & 0x55555555u) << 1u) | ((b & 0xAAAAAAAAu) >> 1u);
	b = ((b & 0x33333333u) << 2u) | ((b & 0xCCCCCCCCu) >> 2u);
	b = ((b & 0x0F0F0F0Fu) << 4u) | ((b & 0xF0F0F0F0u) >> 4u);
	b = ((b & 0x00FF00FFu) << 8u) | ((b & 0xFF00FF00u) >> 8u);

	float radicalInverseVDC = float(b) * 2.3283064365386963e-10;

	return float2((i / numSamples), radicalInverseVDC);
}

float3 ImportanceSampleGGX(float2 Xi, float Roughness, float3 N)
{
	float a = Roughness * Roughness;
	float Phi = 2 * PI * Xi.x;
	float CosTheta = sqrt((1 - Xi.y) / (1 + (a * a - 1) * Xi.y));
	float SinTheta = sqrt(1 - CosTheta * CosTheta);
	float3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;
	float3 UpVector = abs(N.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
	float3 TangentX = normalize(cross(UpVector, N));
	float3 TangentY = cross(N, TangentX);
	// Tangent to world space
	return TangentX * H.x + TangentY * H.y + N * H.z;
}

float3 PrefilterEnvMap(float Roughness, float3 R)
{
	float3 N = R;
	float3 V = R;

	float3 PrefilteredColor = 0;
	float TotalWeight = 0.0f;

	const uint NumSamples = 1024;

	for (uint i = 0; i < NumSamples; i++)
	{
		float2 Xi = Hammersley(i, NumSamples);
		float3 H = ImportanceSampleGGX(Xi, Roughness, N);
		float3 L = 2 * dot(V, H) * H - V;
		float NoL = saturate(dot(N, L));
		if (NoL > 0)
		{
			PrefilteredColor += gEnvMap.SampleLevel(samLinearClamp, L, 0).rgb * NoL;
			TotalWeight += NoL;
		}
	}
	return PrefilteredColor / TotalWeight;
}

float4 IntegrateIrradiance(float3 N)
{
	float3 irradiance = float3(0.0f, 0.0f, 0.0f);

	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 right = cross(up, N);
	up = cross(N, right);

	float sampleDelta = 0.025f;
	float nrSamples = 0.0f;

	for (float phi = 0.0f; phi < 2.0 * PI; phi += sampleDelta)
	{
		for (float theta = 0.0f; theta < 0.5 * PI; theta += sampleDelta)
		{
			float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			float3 sampleVec = (tangentSample.x * right) + (tangentSample.y * up) + (tangentSample.z * N);

			irradiance += gEnvMap.Sample(samLinearClamp, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}

	irradiance = PI * irradiance * (1 / nrSamples);

	return float4(irradiance, 1.0f);
}

float3 FresnelSchlickRoughness(float NoV, float3 specColor, float roughness)
{
	float roughnessPercent = 1.0f - roughness;
	return specColor + (max(roughnessPercent, specColor) - specColor) * pow(1.0 - NoV, 5.0f);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float a = roughness;
	float k = (a * a) / 2.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
// ----------------------------------------------------------------------------
float G_Smith(float roughness, float NdotV, float NdotL)
{
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

// BRDF 맵은 구해다가 사용중
float2 IntegrateBRDF(float Roughness, float NoV)
{
	float3 V;
	V.x = sqrt(1.0f - NoV * NoV);
	V.y = 0;
	V.z = NoV;

	float A = 0;
	float B = 0;

	const uint NumSamples = 1024;

	float3 N = float3(0.0f, 0.0f, 1.0f);

	for (uint i = 0; i < NumSamples; i++)
	{
		float2 Xi = Hammersley(i, NumSamples);
		float3 H = ImportanceSampleGGX(Xi, Roughness, N);
		float3 L = 2 * dot(V, H) * H - V;

		float NoL = saturate(L.z);
		float NoH = saturate(H.z);
		float VoH = saturate(dot(V, H));

		if (NoL > 0)
		{
			float G = G_Smith(Roughness, NoV, NoL);
			float G_Vis = G * VoH / (NoH * NoV);
			float Fc = pow(1 - VoH, 5);

			A += (1 - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}

	return float2(A, B) / NumSamples;
}

float3 IBL(float3 albedo, float roughness, float metallic, float3 N, float3 V)
{
	float NoV = saturate(dot(N, V));
	float3 R = 2 * dot(V, N) * N - V;

	//float3 irradiance = IntegrateIrradiance(N);
	float3 irradiance = gIrradianceMap.Sample(samLinearWrap, N).rgb;
	float3 preFilteredEnv = gEnvMap.SampleLevel(samLinearWrap, R, roughness * 10).rgb;
	float2 brdf = gBRDFMap.Sample(samAnisotropicWrap, float2(max(dot(N, V), 0.0f), roughness)).rg;
	//float3 preFilteredEnv = PrefilterEnvMap(roughness, N);

	float3 F0 = lerp(0.04f, albedo, metallic);

	float3 kS = FresnelSchlickRoughness(max(dot(N, V), 0.0f), F0, roughness);
	float3 specular = preFilteredEnv * (kS * brdf.r + brdf.g);

	float3 kD = (1 - kS) * (1 - metallic);

	float3 diffuse = albedo * irradiance * kD;

	return diffuse + specular;
}

float3 Multiply(const float3 m[3], const float3 v)
{
	float x = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
	float y = m[1][0] * v[1] + m[1][1] * v[1] + m[1][2] * v[2];
	float z = m[2][0] * v[1] + m[2][1] * v[1] + m[2][2] * v[2];
	return float3(x, y, z);
}

float3 rtt_and_odt_fit(float3 v)
{
	float3 a = v * (v + 0.0245786f) - 0.000090537f;
	float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
	return a / b;
}

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
static float3 aces_input_matrix[3] = { float3(0.59719f, 0.35458f, 0.04823f), float3(0.07600f, 0.90834f, 0.01566f), float3(0.02840f, 0.13383f, 0.83777f) };
// ODT_SAT => XYZ => D60_2_D65 => sRGB
static float3 aces_output_matrix[3] = { float3(1.60475f, -0.53108f, -0.07367f), float3(-0.10208f, 1.10813f, -0.00605f), float3(-0.00327f, -0.07276f, 1.07602f) };


float4 Deferred_PS_Light(PS_IN input) : SV_TARGET
{
	float4 depth = gDepth.Sample(samLinearClamp, input.UV);

	clip(depth.x == 1.0f ? -1 : 1);

	float4 albedo = gAlbedo.Sample(samLinearClamp, input.UV);
	float4 normal = gNormal.Sample(samLinearClamp, input.UV);
	float4 worldPos = gWorldPos.Sample(samLinearClamp, input.UV);
	float4 emissive = gEmissive.Sample(samLinearClamp, input.UV);

	float isVantaObject = worldPos.w;

	emissive = pow(emissive, 2.2f);

	float3 _finColor = emissive.rgb;

	if (isVantaObject > 0.)
	{
		float4 shadowH = float4(0., 0., 0., 0.);

		float shadows = 1.0f;

		float roughness = lerp(0.04f, 1.0f, normal.w);
		float metallic = lerp(0.04f, 1.0f, albedo.w);

		albedo.w = 1.0f;
		albedo = pow(albedo, 2.2);

		normal.a = 1.0f;
		normal = (normal - 0.5f) * 2.f;

		float3 toEye = normalize(camera.cameraPosition.xyz - worldPos.xyz);

		float roughness2 = max(.001, roughness * roughness);

		float3 specularColor = albedo.rgb * metallic;
		float3 diffuseColor = albedo.rgb * (1 - metallic);
		float blackLight = 1.f;
		[unroll]
		for (uint i = 0; i < LightCount; i++)
		{
			if (LightInfo[i].Type != 3)
			{
				if (LightInfo[i].Type != 1)
				{
					shadowH = mul(float4(worldPos.xyz, 1.0f), LightInfo[i].ShadowTransform);
					shadowH.xyz /= shadowH.w;

					shadows = CalcShadowFactor(samShadow, gShadow[i], float3(shadowH.xyz));
				}

				_finColor += CalLight(LightInfo[i], specularColor, diffuseColor, worldPos.xyz, normal.xyz, toEye, roughness2, metallic) * shadows;
			}
			else
			{
				float3 lightVec = normalize(LightInfo[i].Position - worldPos.xyz);
				float3 L = normalize(lightVec);
				float cosAng = dot(-LightInfo[i].Direction, L);

				if (cosAng > LightInfo[i].SpotAngle)
				{
					blackLight = CalcFallOff(LightInfo[i].SpotAngle - cosAng, LightInfo[i].SpotAngle - 1);
					blackLight *= blackLight;
				}
			}
		}

		float3 ibl = IBL(albedo.rgb, roughness, metallic, normal.xyz, toEye) /* shadows*/;

		_finColor += ibl * IBLFactor;
		_finColor *= isVantaObject * blackLight;
	}

	/*_finColor = Multiply(aces_input_matrix, _finColor);
	_finColor = rtt_and_odt_fit(_finColor);
	_finColor = Multiply(aces_output_matrix, _finColor);*/

	return float4(_finColor, 1.0f);
}