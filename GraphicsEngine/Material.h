#pragma once

#include "Export/IMaterial.h"

#include "DX11Define.h"

#include "ConstBufferDataInfo.h"

namespace TLGraphicsEngine
{
	class Texture;
	class Shader;

	struct MaterialCB
	{
		MaterialCB() {
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular; // w = SpecPower
		XMFLOAT4 Reflect;
		XMFLOAT4 Emissive;
	};
	
	/// <summary>
	/// Material class
	/// 2022-08-01
	/// </summary>
	
	class Material :
		public IMaterial
	{
	public:
		Material();
		Material(const char* name);
		Material(const Material&) = default;
		Material(Material&&) = default;
		~Material();

		//virtual void Release();

	public:
		MaterialCB m_MaterialCB;

	public:
		// Todo : shader_ptr
		std::shared_ptr<Shader> m_pixelShader;

		std::shared_ptr<Shader> GetShader() { return m_pixelShader; };

		float m_fMetallic = 0.0f;
		float m_fRoughness = 0.0f;

		bool m_SetBumpMap = false;
		bool m_SetAlbedoMap = false;
		
		XMFLOAT4 m_color;
		XMFLOAT2 m_tiling;
		XMFLOAT2 m_offset;

		// Todo : shader_ptr
		std::shared_ptr<Texture> m_mainTex;
		std::shared_ptr<Texture> m_bumpTex;
		std::shared_ptr<Texture> m_metallicGlossMap;
		std::shared_ptr<Texture> m_occlusionMap;

		bool m_setConstBuffer = false;
		PerCustom_PSCB m_perCustumConstBuffer;

	public:
		virtual void SetColor(const char* name, DirectX::SimpleMath::Color value);
		virtual bool SetShader(std::shared_ptr<Shader> pixelShader);
		virtual void SetFloat(const char* name, float value);
		virtual void SetInt(const char* name, int value);
		virtual void SetTexture(const char* name, std::shared_ptr<ITexture> texture);
		virtual void SetTexture(const char* name, UID texture);
		virtual void SetVector(const char* name, DirectX::SimpleMath::Vector4 texture);
		virtual void SetEmissiveColor(DirectX::SimpleMath::Color value);
		virtual void SetAlbedoColor(DirectX::SimpleMath::Color value);


		// 죄악을 짊어진 버퍼
		virtual void SetConstBuffer(void* constBuffer, int size);
		virtual void RemoveConstBuffer();

	public:
		// 메타 나중에 분리 하고 serialized 하고 싶음
		std::string m_MatName;
		std::string m_VShaderID;
		std::string m_PShaderID;

		std::string m_mainTexID = "None";
		std::string m_bumpTexID = "None";
		std::string m_metallicGlossMapID = "None";
		std::string m_occlusionMapID = "None";
	};
}