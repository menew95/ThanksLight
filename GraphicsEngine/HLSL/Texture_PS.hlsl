Texture2D gDiffuseMap : register(t0);

SamplerState samAnisotropicWrap : register(s0)
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct PS_IN
{
	float4 PosH	   : SV_POSITION;
	float2 UV      : UV;
};

// pixel shader
float4 Texture_PS(PS_IN input) : SV_TARGET0
{
	return gDiffuseMap.Sample(samAnisotropicWrap, input.UV);
}