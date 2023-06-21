struct MeshInput
{
	float3 PosL		: POSITION;
	float4 Color	: COLOR;
	float2 UV		: UV;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
};

struct SkinMeshInput
{
	float3 PosL		: POSITION;
	float4 Color	: COLOR;
	float2 UV		: UV;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float4 Bone		: BONE;
	float4 Weight	: WEIGHT;
};

struct QuardInput
{
	float4 PosH	   : SV_POSITION;
	float3 Normal	: NORMAL;
	float2 UV      : UV;
};