#include "pch.h"
#include "Mesh.h"
#include "GraphicResourceManager.h"

TLGraphicsEngine::Mesh::Mesh(const char* name) :
	IMesh(name),
	m_Primitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST), m_BoundingBoxPrimitive(D3D_PRIMITIVE_TOPOLOGY_LINELIST),
	m_Renderstate(0),
	m_MinBox(0, 0, 0),
	m_MaxBox(0, 0, 0)
{

}

TLGraphicsEngine::Mesh::~Mesh()
{

}

ID3D11RasterizerState* TLGraphicsEngine::Mesh::GetRenderstate()
{
	return m_Renderstate.Get();
}

void TLGraphicsEngine::Mesh::CreateMeshBuffer(ID3D11Device* device)
{
	m_vertexBuffer->CreateVertexBuffer(device);

	for (auto& ib : m_indexBuffers)
	{
		ib->CreateIndexBuffer(device);
	}

	if (m_BoundingBoxVertices.size() != 0)
	{
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * static_cast<UINT>(m_BoundingBoxVertices.size());
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = m_BoundingBoxVertices.data();
		HR(device->CreateBuffer(&vbd, &vinitData, m_BoundingBoxVB.GetAddressOf()));

#ifdef _DEBUG
		m_BoundingBoxVB->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(GetName()) - 1, GetName());
#endif // _DEBUG

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * static_cast<UINT>(m_BoundingBoxTriangles.size());
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = m_BoundingBoxTriangles.data();
		HR(device->CreateBuffer(&ibd, &iinitData, m_BoundingBoxIB.GetAddressOf()));

#ifdef _DEBUG
		m_BoundingBoxIB->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(GetName()) - 1, GetName());
#endif // _DEBUG
	}
}

void TLGraphicsEngine::Mesh::SetVertices(std::vector<Vertex>& vers)
{
	m_vertexBuffer = std::make_shared<VertexBuffer>();

	m_vertexBuffer->SetVertices(vers);
}

void TLGraphicsEngine::Mesh::SetTriangles(std::vector<UINT>& tris)
{
	m_indexBuffers.push_back(std::make_shared<IndexBuffer>());

	m_indexBuffers.back()->SetTriangles(tris);
}

void TLGraphicsEngine::Mesh::SetRenderstate(ID3D11RasterizerState* renderState)
{
	m_Renderstate = renderState;
}

void TLGraphicsEngine::Mesh::Bind(ID3D11DeviceContext* deviceContext, int i)
{
	// 버텍스 버퍼, 인덱스 버퍼 바인드
	if (i == 0)
	{
		deviceContext->IASetPrimitiveTopology(m_Primitive);
		m_vertexBuffer->Bind(deviceContext);
	}

	m_indexBuffers[i]->Bind(deviceContext);

}

void TLGraphicsEngine::Mesh::BindBoundingBox(ID3D11DeviceContext* deviceContext)
{
	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, m_BoundingBoxVB.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(m_BoundingBoxIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

bool TLGraphicsEngine::Mesh::GetVertexBufferInfo(Buffer& output)
{
	output.buffer = m_vertexBuffer->GetVertices().data();
	output.bufferCnt = m_vertexBuffer->GetVertexCount();
	output.bufferSize = sizeof(Vertex);

	return true;
}

bool TLGraphicsEngine::Mesh::GetIndexBufferInfo(Buffer& output)
{
	output.buffer = m_indexBuffers[0]->GetTriangles().data();
	output.bufferCnt = m_indexBuffers[0]->GetIndexCount();
	output.bufferSize = sizeof(UINT);

	return true;
}

