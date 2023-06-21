#pragma pack_matrix( row_major )

struct VS_IN
{
	float3 PosL : POSITION;
	float3 Normal : NORMAL;
	float2 UV : UV;
};

struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float2 UV : UV;
};

// vertex shader 
VS_OUT Quard_VS(VS_IN input)
{
	VS_OUT vout;

	// Transform to homogeneous clip space.
	vout.PosH = float4(input.PosL, 1.f);
	vout.UV = input.UV;

	return vout;
}