#pragma pack_matrix( column_major )

SamplerState samAnisotropicWrap : register(s0);
TextureCube gEnvMap	: register(t0);

#define PI 3.14159265359f

float4 IBL_Irradiance_PS(PixelIn pin) : SV_TARGET
{
	float3 normal = normalize(pin.PosL);

	float3 irradiance = float3(0.0f, 0.0f, 0.0f);

	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 right = cross(up, normal);
	up = cross(normal, right);

	float sampleDelta = 0.025f;
	float nrSamples = 0.0f;

	for (float phi = 0.0f; phi < 2.0 * PI; phi += sampleDelta)
	{
		for (float theta = 0.0f; theta < 0.5 * PI; theta += sampleDelta)
		{
			float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			float3 sampleVec = (tangentSample.x * right) + (tangentSample.y * up) + (tangentSample.z * normal);

			irradiance += gEnvMap.Sample(samAnisotropicWrap, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}

	irradiance = PI * irradiance * (1 / nrSamples);

	return float4(irradiance, 1.0f);
}