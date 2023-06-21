#pragma once

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace TLGraphicsEngine
{
	struct Vertex
	{
		Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color)
			: Pos(pos), Color(color), UV(0, 0), Normal(0, 1, 0)
			, Tangent(0, 0, 0), Bone(0, 0, 0, 0), Weight(0, 0, 0, 0)
		{ }
		Vertex() : Pos(0, 0, 0), Color(1, 1, 1, 1), UV(0, 0)
			, Normal(1, 0, 0), Tangent(0, 0, 0), Bone(0, 0, 0, 0)
			, Weight(0, 0, 0, 0)
		{ }
		Vertex(DirectX::XMFLOAT3 pos) : Pos(pos), Color(1, 1, 1, 1), UV(0, 0)
			, Normal(1, 0, 0), Tangent(0, 0, 0), Bone(0, 0, 0, 0)
			, Weight(0, 0, 0, 0)
		{ }
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;

		DirectX::XMFLOAT4 Bone; // 스킨 매쉬용 => 일단 지금은 본이 네개 이하인 것만 고려한다.
		DirectX::XMFLOAT4 Weight; // 스킨 매쉬용 => 일단 지금은 본이 네개 이하인 것만 고려한다.
	};

	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VB;
		std::vector<Vertex> m_Vertices;
		int m_VertexCount;

	public:
		void CreateVertexBuffer(ID3D11Device* device);
		ID3D11Buffer* GetVB() { return m_VB.Get(); }
		ID3D11Buffer** GetVBRef() { return m_VB.GetAddressOf(); }
		int GetVertexCount() { return m_VertexCount; }
		std::vector<Vertex>& GetVertices() { return m_Vertices; }
		void SetVertices(std::vector<Vertex>& vers);
		void Bind(ID3D11DeviceContext* deviceContext);
	};
}

