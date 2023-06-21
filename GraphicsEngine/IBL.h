#pragma once

#include "Export/ResourceBase.h"

struct ID3D11ShaderResourceView;
struct ID3D11Texture3D;
struct ID3D11Texture2D;

namespace TLGraphicsEngine
{
	class Shader;
	class Mesh;
	class Texture;
	class Material;

	/// <summary>
	/// Create InrradianceMap Class
	/// </summary>
	class IBL
	{
	public:
		IBL();
		~IBL();
	
	private:
		// todo : skybox 사이즈 받아와서 크기에 맞게 
		// Prefiltered, Irradiance 맵 사이즈 조절하는 방법이 있는가?
		int m_iSkyBoxSize = 1024;

		shared_ptr<Shader> m_IBL_PrefilteredShaderVS;
		shared_ptr<Shader> m_IBL_PrefilteredShaderPS;
		shared_ptr<Shader> m_IBL_BRDFShaderVS;
		shared_ptr<Shader> m_IBL_BRDFShaderPS;
		shared_ptr<Shader> m_IBL_IrradianceShaderVS;
		shared_ptr<Shader> m_IBL_IrradianceShaderPS;

		shared_ptr<Mesh> m_SkyBoxMesh;

		shared_ptr<Texture> m_PrefilteredTextrue;
		shared_ptr<Texture> m_BRDFTextrue;
		shared_ptr<Texture> m_IrradianceTextrue;



		bool m_bIsDirty = false;

	private:
		bool CreatePregilteredMapSRV();
		bool CreateIrradianceSRV();

		bool CreatePregilteredMap(Texture* skyBox);
		bool CreateBRDFMap();
		bool CreateIrradianceMap(Texture* skyBox);

	public:
		bool IsDirty();
		bool SetIBL(Texture* skyBox);
		
		Texture* GetPregilteredMap();
		Texture* GetBRDFMap();
		Texture* GetIrradianceMap();
	};
}