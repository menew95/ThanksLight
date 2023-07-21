#pragma once

#define MAX_PROJECTED_TEXTURE 6

struct ID3D11DeviceContext;

namespace TLGraphicsEngine
{
	class RenderTargetView;
	class DepthStencilView;
	class Shader;
	struct RenderObject;
	class ProjectedTexture;

	class ProjecedTexturePass
	{
	public:
		ProjecedTexturePass();
		~ProjecedTexturePass();
	
	public:
		int m_projectorCount = 0;

		bool m_bIsMakedTexture = false;
		std::shared_ptr<Shader> m_Texture_VS;
		std::shared_ptr<Shader> m_Texture_PS;
		std::shared_ptr<Shader> m_ProjectedPass_PS;
		std::shared_ptr<Shader> m_Mesh_VS;
		std::shared_ptr<Shader> m_Forward_Light_PS;

		/// <summary>
		/// 매쉬를 포워드로 랜더링(라이트까지 입혀서)
		/// </summary>
		void MakeProjectedTexture(RenderObject* renderObject);
		void RenderToProjectedTexture(RenderObject* renderObject, std::shared_ptr<ProjectedTexture> projected);
		void RecordDepth(RenderTargetView* depthBuffer);
		void RecordDepth(DepthStencilView* depthBuffer);
		void RecordDepthBuffer(RenderTargetView* depthBuffer, std::shared_ptr<ProjectedTexture> projected);
		void RecordDepthBuffer(DepthStencilView* depthBuffer, std::shared_ptr<ProjectedTexture> projected);

		/// <summary>
		/// 현재 2D화가 된 텍스처들을 출력
		/// </summary>
		void SetPass(RenderTargetView* worldRTV, RenderTargetView* target);
		void RenderProjector(ID3D11DeviceContext* deviceContext, RenderTargetView* worldRTV, RenderTargetView* target, std::shared_ptr<ProjectedTexture> projected);

		std::vector<std::shared_ptr<ProjectedTexture>> m_projectedTextures;

		void Reset();
		void OnResize(int width, int height);
	};
}