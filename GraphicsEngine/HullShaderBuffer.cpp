#include "pch.h"
#include "HullShaderBuffer.h"

TLGraphicsEngine::HullShaderBuffer::HullShaderBuffer()
{

}

TLGraphicsEngine::HullShaderBuffer::~HullShaderBuffer()
{

}

void* TLGraphicsEngine::HullShaderBuffer::GetShader()
{
	return m_hullShader.Get();
}

void TLGraphicsEngine::HullShaderBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->HSSetShader(m_hullShader.Get(), nullptr, 0);
}
