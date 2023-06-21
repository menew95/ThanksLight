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
		// �ֺ� ������ �����´�. 12���� ���� ��ճ��� �ʹ�.
		Pixel = gDiffuseMap.Sample(samLinearClamp, input.UV + (g_Filter[i].xy) * g_TexelSize) * 1.0f / 12.0f;

		// 0���� 1�� ������ �ٲ� �ش�.
		// Clamps the specified value within the range of 0 to 1.
		Pixel = saturate(Pixel);

		// 12���� ��������� ���� �ʹ�.
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
		// ����ġ ���ø��� �ϸ�, ��Ⱑ �����ϰ� ���ȴ�. �̰��� ������ ������, saturate�ϸ� ���� �κи� �߷�����.
		Pixel = gDiffuseMap.Sample(samLinearClamp, input.UV + (g_Filter[i].xy) * g_TexelSize * 2.0f) - 0.5f;

		// Clamps the specified value within the range of 0 to 1.
		Pixel = saturate(Pixel);

		// �׷��� ������ ���� �κ��� ����ġ�� �� �ش�.
		AddColor += Pixel * g_BloomWeight;
	}


	// Default to multiplicative identity.
	float4 texColor = float4(1, 1, 1, 1);

	texColor = gDiffuseMap.Sample(samLinearClamp, input.UV) + AddColor;

	return texColor;
}