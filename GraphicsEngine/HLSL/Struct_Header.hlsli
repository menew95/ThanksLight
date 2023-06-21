struct Material
{
	float4 diffuse;
	float4 emissive;
	float roughness;
	float metallic;
	float2 tiling;
	int isUseAlbedoTex;
	int isUseBumpTex;
};

struct CameraInfo
{
	float4 cameraPosition;
	matrix camWorld;
	matrix view;
	matrix proj;
	matrix projInv;
	matrix viewProj;
	matrix viewToTexSpace;
	matrix camWorldViewProj;
};

struct SSAOInfo
{
	float4 offSetVectors[14];
	float4 frustumCorners[4];

	float occlusionRadius;
	float occlusionFadeStart;
	float occlusionFadeEnd;
	float surfaceEpsilon;
};