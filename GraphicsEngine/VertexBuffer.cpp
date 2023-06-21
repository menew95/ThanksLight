#include "pch.h"
#include "VertexBuffer.h"

TLGraphicsEngine::VertexBuffer::VertexBuffer()
: m_VB(nullptr)
, m_VertexCount(0)
{

}

TLGraphicsEngine::VertexBuffer::~VertexBuffer()
{

}

void TLGraphicsEngine::VertexBuffer::CreateVertexBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * m_VertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = m_Vertices.data();
	HR(device->CreateBuffer(&vbd, &vinitData, m_VB.GetAddressOf()));
}

void TLGraphicsEngine::VertexBuffer::SetVertices(std::vector<Vertex>& vers)
{
	m_VertexCount = (int)vers.size();

	m_Vertices.resize(m_VertexCount);

	m_Vertices.assign(vers.begin(), vers.end());
}

void TLGraphicsEngine::VertexBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);
}
