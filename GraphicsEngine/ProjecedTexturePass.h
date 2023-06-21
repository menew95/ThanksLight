#pragma once

#define MAX_PROJECTED_TEXTURE 6

struct ID3D11DeviceContext;

namespace TLGraphicsEngine
{
	class RenderTargetView;
	class DepthStencil;
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

		bool makedTexture = false;
		std::shared_ptr<Shader> m_Texture_VS;
		std::shared_ptr<Shader> m_Texture_PS;
		std::shared_ptr<Shader> m_ProjectedPass_PS;
		std::shared_ptr<Shader> m_MeshVS;
		std::shared_ptr<Shader> m_Forward_Light_PS;

		/// <summary>
		/// �Ž��� ������� ������(����Ʈ���� ������)
		/// 
		/// </summary>


		void MakeProjectedTexture(RenderObject* renderObject);
		void RenderToProjectedTexture(RenderObject* renderObject, std::shared_ptr<ProjectedTexture> projected);
		void RecordDepth(RenderTargetView* depthBuffer);
		void RecordDepthBuffer(RenderTargetView* depthBuffer, std::shared_ptr<ProjectedTexture> projected);

		/// <summary>
		/// ���� 2Dȭ�� �� �ؽ�ó���� ���
		/// </summary>
		void SetPass(RenderTargetView* worldRTV, RenderTargetView* target);
		void RenderProjector(ID3D11DeviceContext* deviceContext, RenderTargetView* worldRTV, RenderTargetView* target, std::shared_ptr<ProjectedTexture> projected);

		std::vector<std::shared_ptr<ProjectedTexture>> m_projectedTextures;

		void Reset();
		void OnResize(int width, int height);
	};
}