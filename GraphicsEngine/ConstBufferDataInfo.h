#pragma once

#include <Windows.h>
#include <DirectXMath.h>

#include "ConstBufferManager.h"

#define MAX_LIGHT_COUNT 20

using namespace DirectX;

namespace TLGraphicsEngine
{
	struct CBBase
	{
	
	};

	struct alignas(16) PostProcess_Blur_PSCB : CBBase
	{
		PostProcess_Blur_PSCB() {
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT4 BlurInfo;
	};

	struct alignas(16) PostProcess_Up_PSCB : CBBase
	{
		PostProcess_Up_PSCB() {
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT4 UpInfo;
	};

	struct alignas(16) PostProcess_Bloom_PSCB : CBBase
	{
		PostProcess_Bloom_PSCB() {
			ZeroMemory(this, sizeof(this));
		}

		float Threshold_Min;
		float Threshold_Max;
	};

	struct alignas(16) PostProcess_ToneMapping_PSCB : CBBase
	{
		PostProcess_ToneMapping_PSCB() {
			ZeroMemory(this, sizeof(this));
		}

		float g_fHardExposure;
	};

	struct alignas(16) CameraInfo
	{
		CameraInfo() {
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT3 cameraPosision;
		float pad;
		XMFLOAT4X4 camWorld;
		XMFLOAT4X4 view;
		XMFLOAT4X4 proj;
		XMFLOAT4X4 projInv;
		XMFLOAT4X4 viewProj;
		XMFLOAT4X4 viewToTexSpace;
		XMFLOAT4X4 camWorldViewProj;
	};

	struct alignas(16) SSAOInfo
	{
		XMFLOAT4 offSetVectors[14];
		XMFLOAT4 frustumCorners[4];

		float occlusionRadius = 0.5f;
		float occlusionFadeStart = 0.2f;
		float occlusionFadeEnd = 2.0f;
		float surfaceEpsilon = 0.05f;
	};

	struct alignas(16) LightInfo
	{
		LightInfo() { ZeroMemory(this, sizeof(this)); }

		int			m_iType;
		float		m_fRange;
		float		m_fFallOff;
		float		pad;

		XMFLOAT3	m_Position;
		float		m_fSpotAngle;

		XMFLOAT3	m_Direction;
		float		m_fFallOffAngle;

		XMFLOAT3	m_Color;
		float		m_fPower;

		XMFLOAT4X4	m_ShadowTransform;
	};

	struct alignas(16) Light_CB : CBBase
	{
		Light_CB() { 
			ZeroMemory(this, sizeof(this));
		}

		unsigned int LightCount;
		float IBLFactor;
		XMFLOAT2 pad;

		LightInfo LightInfo[MAX_LIGHT_COUNT];
	};

	struct alignas(16) PerFrame_CB : CBBase
	{
		PerFrame_CB() {
			ZeroMemory(this, sizeof(this));
		}

		CameraInfo cameraInfo;

		SSAOInfo ssaoInfo;
	};

	struct alignas(16) PerObject_CB : CBBase
	{
		PerObject_CB() {
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT4X4 world;
		XMFLOAT4X4 worldView;
		XMFLOAT4X4 worldInvTranspose;
		XMFLOAT4X4 worldViewProj;
		XMFLOAT4X4 texTransform;

		XMFLOAT4X4 boneTransforms[64];
	};

	struct alignas(16) MaterialInfo
	{
		MaterialInfo() {
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT4 diffuse;
		XMFLOAT4 emissive;

		float roughness;
		float metallic;
		XMFLOAT2 m_tiling;

		int isUseAlbedo;
		int isUseBump;
		XMFLOAT2 pad;
	};

	struct alignas(16) PerMaterial_CB : CBBase
	{
		PerMaterial_CB() {
			ZeroMemory(this, sizeof(this));
		}

		MaterialInfo material;
	};

	struct alignas(16) VANTA_PBR_PSCB : CBBase
	{
		VANTA_PBR_PSCB() {
			ZeroMemory(this, sizeof(this));
		}
		float SpotAngle;
		XMFLOAT3 Direction;
		bool boolPad[4];
		int isCamLight;
		int pad[3];
	};

	struct alignas(16) Projected_PSCB : CBBase
	{
		Projected_PSCB() {
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT4X4 camViewProj;
	};

	struct alignas(16) PerCustom_PSCB : CBBase
	{
		PerCustom_PSCB() {
			ZeroMemory(this, sizeof(this));
		}
		
		float floatValue1;
		float floatValue2;
		float floatValue3;
		float floatValue4;
		int intValue1;
		int intValue2;
		int intValue3;
		int intValue4;
	};

	struct alignas(16) PostProcess_OutLine_PSCB : CBBase
	{
		PostProcess_OutLine_PSCB() {
			ZeroMemory(this, sizeof(this));
		}

		float thickNess; // 1
		float minDepth; // 0
		float maxDepth; // 1
		float pad;
		XMFLOAT2 mainTex_TexelSize;
		XMFLOAT4 orthoParams;		// x: width, y: height, z: unused, w: ortho ? 1 : 0
		XMFLOAT4 zBufferParams;		// x: 1-far/near, y: far/near, z: x/far, w: y/far
		XMFLOAT4 projectionParams;	// x: 1 (-1 flipped), y: near, z: far, w: 1/far
	};
}