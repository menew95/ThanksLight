#pragma once
#include "Export/IMesh.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ID3D11Buffer;
struct ID3D11RasterizerState;
enum D3D_PRIMITIVE_TOPOLOGY;

namespace TLGraphicsEngine
{
	
	class Mesh :
		public IMesh
	{
	public:
		Mesh(const char* name);
		Mesh(const Mesh&) = default;
		Mesh(Mesh&&) noexcept = default;
		Mesh& operator=(Mesh&&) = default;
		virtual ~Mesh();

		//void Release() override;
	public:
		bool m_SkinnedMesh = false;

		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::vector<std::shared_ptr<IndexBuffer>> m_indexBuffers;

		ComPtr<ID3D11RasterizerState> m_Renderstate;

		D3D_PRIMITIVE_TOPOLOGY m_Primitive;

	public:
		/// <summary>
		/// BoundingBox 용 데이터들
		/// </summary>
		DirectX::XMFLOAT3 m_MinBox;
		DirectX::XMFLOAT3 m_MaxBox;

		ComPtr<ID3D11Buffer> m_BoundingBoxVB;
		ComPtr<ID3D11Buffer> m_BoundingBoxIB;

		std::vector<Vertex> m_BoundingBoxVertices;
		std::vector<UINT> m_BoundingBoxTriangles;

		bool m_IsPicking = false;

		D3D_PRIMITIVE_TOPOLOGY m_BoundingBoxPrimitive;

	public:
		ID3D11RasterizerState* GetRenderstate();

		void CreateMeshBuffer(ID3D11Device* device);

		std::shared_ptr<VertexBuffer>& GetVertexBuffer() { return m_vertexBuffer; }
		std::vector<std::shared_ptr<IndexBuffer>>& GetIndexBuffers() { return m_indexBuffers; }

		void SetVertices(std::vector<Vertex>& vers);
		void SetTriangles(std::vector<UINT>& tris);
		void SetRenderstate(ID3D11RasterizerState * renderState);

		void Bind(ID3D11DeviceContext* deviceContext, int i);
		void BindBoundingBox(ID3D11DeviceContext* deviceContext);

		virtual bool GetVertexBufferInfo(Buffer& output) override;
		virtual bool GetIndexBufferInfo(Buffer& output) override;
	};
}