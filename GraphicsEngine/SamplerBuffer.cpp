#include "pch.h"
#include "SamplerBuffer.h"
#include "Shader.h"
#include "GraphicsEngine.h"
#include "SamplerState.h"

TLGraphicsEngine::SamplerBuffer::SamplerBuffer(const char* name, int bindPoint, int bindCnt)
:
ShaderResourceBase(name, bindPoint, bindCnt, RegisterType::Sampler, ResourceType::ETC)
{
	m_pSamplerState = SamplerState::Instance()->GetSamplerState(name);
}

TLGraphicsEngine::SamplerBuffer::~SamplerBuffer()
{

}

void TLGraphicsEngine::SamplerBuffer::Update(ShaderType type)
{
	auto* deviceContext = GraphicsEngine::Instance()->GetDeviceContext();

	if (m_pSamplerState == nullptr)
	{
		return;
	}

	switch (type)
	{
		case ShaderType::Pixel:
		{
			deviceContext->PSSetSamplers(m_BindPoint, m_BindCnt, m_pSamplerState.GetAddressOf());
			break;
		}
		case ShaderType::Vertex:
		{
			deviceContext->VSSetSamplers(m_BindPoint, m_BindCnt, m_pSamplerState.GetAddressOf());
			break;
		}
		case ShaderType::Geometry:
		{
			deviceContext->GSSetSamplers(m_BindPoint, m_BindCnt, m_pSamplerState.GetAddressOf());
			break;
		}
		case ShaderType::Hull:
		{
			deviceContext->HSSetSamplers(m_BindPoint, m_BindCnt, m_pSamplerState.GetAddressOf());
			break;
		}
		case ShaderType::Domain:
		{
			deviceContext->DSSetSamplers(m_BindPoint, m_BindCnt, m_pSamplerState.GetAddressOf());
			break;
		}
		case ShaderType::Compute:
		{
			deviceContext->CSSetSamplers(m_BindPoint, m_BindCnt, m_pSamplerState.GetAddressOf());
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

void TLGraphicsEngine::SamplerBuffer::Update(ShaderType type, ID3D11SamplerState* sampler)
{
	auto* deviceContext = GraphicsEngine::Instance()->GetDeviceContext();

	switch (type)
	{
		case ShaderType::Pixel:
		{
			deviceContext->PSSetSamplers(m_BindPoint, m_BindCnt, &sampler);
			break;
		}
		case ShaderType::Vertex:
		{
			deviceContext->VSSetSamplers(m_BindPoint, m_BindCnt, &sampler);
			break;
		}
		case ShaderType::Geometry:
		{
			deviceContext->GSSetSamplers(m_BindPoint, m_BindCnt, &sampler);
			break;
		}
		case ShaderType::Hull:
		{
			deviceContext->HSSetSamplers(m_BindPoint, m_BindCnt, &sampler);
			break;
		}
		case ShaderType::Domain:
		{
			deviceContext->DSSetSamplers(m_BindPoint, m_BindCnt, &sampler);
			break;
		}
		case ShaderType::Compute:
		{
			deviceContext->CSSetSamplers(m_BindPoint, m_BindCnt, &sampler);
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
