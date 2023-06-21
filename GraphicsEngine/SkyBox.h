#pragma once

#include "Export/ResourceBase.h"

namespace TLGraphicsEngine
{
	class Shader;
	class Mesh;
	class Texture;
	class Material;
	class EngineCamera;

	/// <summary>
	/// 큐브맵 클래스
	/// </summary>
	
	class SkyBox
	{
	public:
		SkyBox(EngineCamera* engineCamera);
		SkyBox(EngineCamera* engineCamera, Mesh* mesh, Material* material/*, Texture* texture*/);
		~SkyBox();

	public:
		EngineCamera* m_EngineCamera = nullptr;

		ID3D11SamplerState* m_SkyBoxSampler = nullptr;

		shared_ptr<Mesh> m_SkyBoxMesh;

		shared_ptr<Shader> m_SkyBoxVS;
		shared_ptr<Shader> m_SkyBoxPS;
		shared_ptr<Texture> m_SkyBoxTextrue;
		shared_ptr<Texture> m_BRDFTextrue;
		shared_ptr<Texture> m_IrradianceTextrue;
 
	public:
		void Initialize();

		void Render();
	};
}

