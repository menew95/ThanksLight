#pragma once

struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;

namespace TLGraphicsEngine
{
	class RenderTargetView;
	class EngineCamera;
	class Shader;
	class IMesh;
	struct RenderObject;
	struct GizmoObject;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

	private:
		static Renderer* m_instance;

	public:
		static Renderer* Instance();
		static void Release();

	public:
		void Initialize(EngineCamera* camera
			, shared_ptr<Shader> staticVS, shared_ptr<Shader> skinVS
			, shared_ptr<Shader> quardVS, shared_ptr<Shader> texturePS
			, shared_ptr<Shader> gizmoVS, shared_ptr<Shader> gizmoPS);
		// Render Object
		void RenderMesh(ID3D11DeviceContext* deviceContext, RenderObject* renderObject);
		void RenderSkinnedMesh(ID3D11DeviceContext* deviceContext, RenderObject* renderObject);
		void Render2D(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* srv);
		void RenderGizmo(ID3D11DeviceContext* deviceContext, GizmoObject* gizmoObject);
		void RenderBoundingBox(ID3D11DeviceContext* deviceContext, IMesh* mesh, float WM[16]);

		void RenderOutLine(ID3D11DeviceContext* deviceContext, RenderObject* renderObject);
	private:
		EngineCamera* m_engineCamera;
		shared_ptr<Shader> m_staticVS;
		shared_ptr<Shader> m_skinVS;
		shared_ptr<Shader> m_quardVS;
		shared_ptr<Shader> m_texturePS;
		shared_ptr<Shader> m_gizmoVS;
		shared_ptr<Shader> m_gizmoPS;

		shared_ptr<Shader> m_outLineVS;
		shared_ptr<Shader> m_outLinePS;
	};
}