#include "pch.h"
#include "GeometryShaderBuffer.h"

TLGraphicsEngine::GeometryShaderBuffer::GeometryShaderBuffer()
{

}

TLGraphicsEngine::GeometryShaderBuffer::~GeometryShaderBuffer()
{

}

void* TLGraphicsEngine::GeometryShaderBuffer::GetShader()
{
	return m_geometryShader.Get();
}

void TLGraphicsEngine::GeometryShaderBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->GSSetShader(m_geometryShader.Get(), nullptr, 0);
}