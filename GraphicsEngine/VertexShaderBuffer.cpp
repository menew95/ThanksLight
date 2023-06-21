#include "pch.h"
#include "VertexShaderBuffer.h"

TLGraphicsEngine::VertexShaderBuffer::VertexShaderBuffer()
: m_vertexShaderBuffer(nullptr)
{

}

TLGraphicsEngine::VertexShaderBuffer::~VertexShaderBuffer()
{

}

void* TLGraphicsEngine::VertexShaderBuffer::GetShader()
{
	return m_vertexShaderBuffer.Get();
}

void TLGraphicsEngine::VertexShaderBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetShader(m_vertexShaderBuffer.Get(), nullptr, 0);
}
