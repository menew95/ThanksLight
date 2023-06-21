#include "pch.h"
#include "ShaderResourceBuffer.h"
#include "Shader.h"
#include "GraphicsEngine.h"

TLGraphicsEngine::ShaderResourceBuffer::ShaderResourceBuffer(const char* name, int bindPoint, int bindCnt)
:
ShaderResourceBase(name, bindPoint, bindCnt, RegisterType::Texture, ResourceType::Texture)
{

}

TLGraphicsEngine::ShaderResourceBuffer::~ShaderResourceBuffer()
{

}

void TLGraphicsEngine::ShaderResourceBuffer::Update(ShaderType type, ID3D11ShaderResourceView** srv)
{
	auto* deviceContext = GraphicsEngine::Instance()->GetDeviceContext();

	if (srv == nullptr)
	{
		return;
	}

	switch (type)
	{
		case ShaderType::Pixel:
		{
			deviceContext->PSSetShaderResources(m_BindPoint, m_BindCnt, srv);
			break;
		}
		case ShaderType::Vertex:
		{
			deviceContext->VSSetShaderResources(m_BindPoint, m_BindCnt, srv);
			break;
		}
		case ShaderType::Geometry:
		{
			deviceContext->GSSetShaderResources(m_BindPoint, m_BindCnt, srv);
			break;
		}
		case ShaderType::Hull:
		{
			deviceContext->HSSetShaderResources(m_BindPoint, m_BindCnt, srv);
			break;
		}
		case ShaderType::Domain:
		{
			deviceContext->DSSetShaderResources(m_BindPoint, m_BindCnt, srv);
			break;
		}
		case ShaderType::Compute:
		{
			deviceContext->CSSetShaderResources(m_BindPoint, m_BindCnt, srv);
			break;
		}
		case ShaderType::Reserved0:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}
