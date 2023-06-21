#include "pch.h"
#include "DomainShaderBuffer.h"

TLGraphicsEngine::DomainShaderBuffer::DomainShaderBuffer()
{

}

TLGraphicsEngine::DomainShaderBuffer::~DomainShaderBuffer()
{

}

void* TLGraphicsEngine::DomainShaderBuffer::GetShader()
{
	return m_domainShaderBuffer.Get();
}

void TLGraphicsEngine::DomainShaderBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->DSSetShader(m_domainShaderBuffer.Get(), nullptr, 0);
}
