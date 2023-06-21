#include "pch.h"
#include "ComputeShaderBuffer.h"

TLGraphicsEngine::ComputeShaderBuffer::ComputeShaderBuffer()
{

}

TLGraphicsEngine::ComputeShaderBuffer::~ComputeShaderBuffer()
{

}

void* TLGraphicsEngine::ComputeShaderBuffer::GetShader()
{
	return m_computeShaderBuffer.Get();
}

void TLGraphicsEngine::ComputeShaderBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->CSSetShader(m_computeShaderBuffer.Get(), nullptr, 0);
}
