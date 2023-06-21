#include "Struct_Header.hlsli"
#include "Light_Header.hlsli"

cbuffer cbPerFrame : register(b0)
{
	CameraInfo camera;
	
	SSAOInfo ssaoInfo;
};

cbuffer cbLight : register(b1)
{
	uint LightCount;
	float IBLFactor;
	float2 pad;

	Light LightInfo[MAX_LIGHT_COUNT];
}

//#ifdef STATIC
cbuffer cbPerObject : register(b2)
{
	float4x4 world;
	float4x4 worldView;
	float4x4 worldInvTranspose;
	float4x4 worldViewProj;
	float4x4 texTransform;

	float4x4 boneTransforms[64];
};

cbuffer cbPerMaterial : register(b2)
{
	Material material;
}

// ÁË¾ÇÀ» Áû¾îÁø ¹öÆÛ
cbuffer cbControlPerFrameMaterial : register(b3)
{
	float floatValue1;
	float floatValue2;
	float floatValue3;
	float floatValue4;
	int intValue1;
	int intValue2;
	int intValue3;
	int intValue4;
};

//// ÁË¾ÇÀ» Áû¾îÁø ¹öÆÛ
//cbuffer cbCustomPerObjectMaterial : register(b4)
//{
//	float floatValue1;
//	float floatValue2;
//	float floatValue3;
//	float floatValue4;
//	int intValue1;
//	int intValue2;
//	int intValue3;
//	int intValue4;
//};

//#endif

//#ifdef SKIN
//cbuffer cbSkinedPerObject : register(b1)
//{
//	float4x4 world;
//	float4x4 worldView;
//	float4x4 worldInvTranspose;
//	float4x4 worldViewProj;
//	float4x4 texTransform;
//
//	Material material;
//
//	float4x4 boneTransforms[64];
//};
//#endif

cbuffer cbBlur : register(b1)
{
	// 0 => x, 1 => y
	float4 blurInfo;
};

cbuffer cbBloom : register(b1)
{
	float threshold_Min;
	float threshold_Max;
};

cbuffer cbOutLine : register(b1)
{
	float thickNess; // 1
	float minDepth; // 0
	float maxDepth; // 1
	float2 mainTex_TexelSize;
	float4 orthoParams;			// x: width, y: height, z: unused, w: ortho ? 1 : 0
	float4 zBufferParams;		// x: 1-far/near, y: far/near, z: x/far, w: y/far
	float4 projectionParams;	// x: 1 (-1 flipped), y: near, z: far, w: 1/far
};