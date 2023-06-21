#pragma once

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace TLGraphicsEngine
{
	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IB;
		std::vector<UINT> m_Triangles;
		int m_IndextexCount = 0;

	public:
		void CreateIndexBuffer(ID3D11Device* device);

		ID3D11Buffer* GetIB() { return m_IB.Get(); }
		ID3D11Buffer** GetIBRef() { return m_IB.GetAddressOf(); }

		int GetIndexCount() { return m_IndextexCount; }

		std::vector<UINT>& GetTriangles() { return m_Triangles; }
		void SetTriangles(std::vector<UINT>& tris);
		void Bind(ID3D11DeviceContext* deviceContext);
	};
}