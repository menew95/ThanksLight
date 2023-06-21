// cross type, size 3
static float3 g_Filter[12] =
{
	-3, 0, 0.111, -2, 0, 0.250, -1, 0, 1.000,
	+3, 0, 0.111, +2, 0, 0.250, +1, 0, 1.000,

	 0, -3, 0.111, 0, -2, 0.250, 0, -1, 1.000,
	 0, +3, 0.111, 0, +2, 0.250, 0, +1, 1.000
};

float2 g_TexelSize;

float g_BloomWeight = 6.0f / 32.0f;
float g_BloomHue = 2.25f;

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap : register(t0);

SamplerState samLinearClamp : register(s0);

struct PS_IN
{
	float4 PosH : SV_POSITION;
	float2 UV : UV;
};

//--------------------------------------------------
// Pixel Shader - Simple Blur
//--------------------------------------------------
float4 PostProcess_BlurPS(PS_IN input) : SV_Target0
{
	float4 AverageColor = 0;
	float4 Pixel = 0;

	for (int i = 0; i < 12; ++i)
	{
		// 주변 샘플을 가져온다. 12개의 값을 평균내고 싶다.
		Pixel = gDiffuseMap.Sample(samLinearClamp, input.UV + (g_Filter[i].xy) * g_TexelSize) * 1.0f / 12.0f;

		// 0에서 1의 범위로 바꿔 준다.
		// Clamps the specified value within the range of 0 to 1.
		Pixel = saturate(Pixel);

		// 12개를 누적평균을 내고 싶다.
		AverageColor += Pixel;
	}

	return AverageColor;
}

//--------------------------------------------------
// Pixel Shader - Bloom type (+Blur)
//--------------------------------------------------
float4 PS_Bloom(PS_IN input) : SV_Target0
{
	float4 AddColor = 0;
	float4 Pixel = 0;

	for (int i = 0; i < 12; ++i)
	{
		// 가중치 샘플링을 하면, 밝기가 과다하게 계산된다. 이것을 적당히 내리고, saturate하면 밝은 부분만 추려진다.
		Pixel = gDiffuseMap.Sample(samLinearClamp, input.UV + (g_Filter[i].xy) * g_TexelSize * 2.0f) - 0.5f;

		// Clamps the specified value within the range of 0 to 1.
		Pixel = saturate(Pixel);

		// 그렇게 구해진 밝은 부분을 가중치를 또 준다.
		AddColor += Pixel * g_BloomWeight;
	}


	// Default to multiplicative identity.
	float4 texColor = float4(1, 1, 1, 1);

	texColor = gDiffuseMap.Sample(samLinearClamp, input.UV) + AddColor;

	return texColor;
}