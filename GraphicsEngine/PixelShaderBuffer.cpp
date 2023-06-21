#include "pch.h"
#include "PixelShaderBuffer.h"

TLGraphicsEngine::PixelShaderBuffer::PixelShaderBuffer()
{

}

TLGraphicsEngine::PixelShaderBuffer::~PixelShaderBuffer()
{

}

void* TLGraphicsEngine::PixelShaderBuffer::GetShader()
{
	return m_pixelShaderBuffer.Get();
}

void TLGraphicsEngine::PixelShaderBuffer::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShader(m_pixelShaderBuffer.Get(), nullptr, 0);
}
