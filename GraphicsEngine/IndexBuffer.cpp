#include "pch.h"
#include "IndexBuffer.h"

TLGraphicsEngine::IndexBuffer::IndexBuffer()
	: m_IndextexCount(0)
{

}

TLGraphicsEngine::IndexBuffer::~IndexBuffer()
{

}


void TLGraphicsEngine::IndexBuffer::CreateIndexBuffer(ID3D11Device* device)
{
	if (m_IndextexCount < 1)
	{
		return;
	}
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * m_IndextexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &m_Triangles.front();

	HRESULT hr = device->CreateBuffer(
		&ibd, 
		&iinitData, 
		m_IB.GetAddressOf());

	//HR(device->CreateBuffer(&ibd, &iinitData, m_IB.GetAddressOf()));
}

void TLGraphicsEngine::IndexBuffer::SetTriangles(std::vector<UINT>& tris)
{
	m_IndextexCount = (int)tris.size();

	m_Triangles.resize(m_IndextexCount);

	m_Triangles.assign(tris.begin(), tris.end());
}

void TLGraphicsEngine::IndexBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}
