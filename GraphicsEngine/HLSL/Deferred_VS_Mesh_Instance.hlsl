#pragma pack_matrix( row_major )

struct VS_IN
{
	float3 PosL		: POSITION;
	float4 Color	: COLOR;
	float2 UV		: UV;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
};

struct Instance_IN
{
	uint InstanceId : SV_InstanceID;
	row_major float4x4 World : WORLD;
};

struct VS_OUT
{
	float4 PosH		: SV_POSITION;
	float4 Color	: COLOR;
	float2 UV		: UV;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;

	float3 PosV		: POSITION1;
	float3 PosW		: POSITION2;
};

cbuffer cbPerObject : register(b0)
{
	float4x4 gView;
	float4x4 gViewProj;
};

VS_OUT Deferred_VS_Mesh(VS_IN input, Instance_IN instance)
{
	VS_OUT output;

	float4x4 worldInvTranspose

	output.Color = input.Color;
	output.UV = input.UV;
	output.Normal = normalize(mul(input.Normal, (float3x3)gWorldInvTranspose));
	output.Tangent = normalize(mul(input.Tangent, (float3x3)instance.World));

	output.PosV = mul(float4(input.PosL, 1.0f), gWorldView).xyz;
	output.PosW = mul(float4(input.PosL, 1.0f), instance.World).xyz;

	output.PosH = mul(float4(output.PosW, 1.0f), gViewProj);
	return output;
}