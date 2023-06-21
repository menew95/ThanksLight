#pragma pack_matrix(row_major)

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
	float PI = 3.14159265359f;
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

float G_Smith(float a2, float NoV, float NoL)
{
	float Vis_SmithV = NoV + sqrt(NoV * (NoV - NoV * a2) + a2);
	float Vis_SmithL = NoL + sqrt(NoL * (NoL - NoL * a2) + a2);
	return  4 * NoV * NoL / (Vis_SmithV * Vis_SmithL);
}

float3 SpecularIBL(float3 SpecularColor, float Roughness, float3 N, float3 V)
{
	float3 SpecularLighting = 0;
	const uint NumSamples = 1024;
	for (uint i = 0; i < NumSamples; i++)
	{
		float2 Xi = Hammersley(i, NumSamples);

		float3 H = ImportanceSampleGGX(Xi, Roughness, N);
		float3 L = 2 * dot(V, H) * H - V;
		float NoV = saturate(dot(N, V));
		float NoL = saturate(dot(N, L));
		float NoH = saturate(dot(N, H));
		float VoH = saturate(dot(V, H));
		if (NoL > 0)
		{
			float3 SampleColor = EnvMap.SampleLevel(samAnisotropicClamp, L, 0).rgb;
			float G = G_Smith(Roughness, NoV, NoL);
			float Fc = pow(1 - VoH, 5);
			float3 F = (1 - Fc) * SpecularColor + Fc;
			// Incident light = SampleColor * NoL
			// Microfacet specular = D*G*F / (4*NoL*NoV)
			// pdf = D * NoH / (4 * VoH)
			SpecularLighting += SampleColor * F * G * VoH / (NoH * NoV);
		}
	}
	return SpecularLighting / NumSamples;
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
			PrefilteredColor += EnvMap.SampleLevel(samAnisotropicClamp, L, 0).rgb * NoL;
			TotalWeight += NoL;
		}
	}
	return PrefilteredColor / TotalWeight;
}

#define MAX_REF_LOD 4.0

float3 FresnelSchlickRoughness(float NdotV, float3 F0, float roughness)
{
	float roughnessPercent = 1.0f - roughness;
	return F0 + (max(float3(roughnessPercent, roughnessPercent, roughnessPercent), F0) - F0) * pow(1.0 - NdotV, 5.0f);
}

float3 IBL_EnvironmentLight(in float3 V, in float3 N, in float3 irradiance, in float3 prefilterColor, in float2 brdf, in float3 albedo, in float ao, in float roughness, in float metallic, in float shadow)
{
	float3 F0 = lerp(F_ZERO, albedo, metallic);

	float3 kS = FresnelSchlickRoughness(max(dot(N, V), 0.0f), F0, roughness);
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	kD *= 1.0 - metallic;

	float3 diffuse = albedo * irradiance * kD;

	float3 specular = prefilterColor * (kS * brdf.x + brdf.y);

	return (diffuse + specular) * ao;
}

float4 Light_IBL_PS(PixelIn pin) : SV_TARGET
{
	float3 irradiance = gIBLIrradiance.Sample(gSamWrapLinear, normal).rgb;
	float3 prefilteredColor = gIBLPrefilter.SampleLevel(gSamWrapLinear, reflect(-ViewDirection, normal), roughness * MAX_REF_LOD).rgb;
	float2 brdf = gBRDFlut.Sample(gSamWrapLinear, float2(max(dot(normal, ViewDirection), 0.0f), roughness)).rg;

	litColor += IBL_EnvironmentLight(ViewDirection, normal, irradiance, prefilteredColor, brdf,
										albedo, ao, roughness, metallic, shadows);

	return float4(litColor, 1.0f);
}