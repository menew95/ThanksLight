#include "pch.h"
#include "ConstBuffer.h"
#include "Shader.h"
#include "GraphicsEngine.h"

TLGraphicsEngine::ConstBuffer::ConstBuffer(const char* name, int size, int bindPoint, int bindCnt)
:
BufferBase(name, bindPoint, bindCnt, size, RegisterType::ConstBuffer)
{
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.ByteWidth = size;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	GraphicsEngine::Instance()->GetDevice()->CreateBuffer(&desc, nullptr,
		&m_Buffer);

#ifdef _DEBUG
	m_Buffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(name) - 1, name);
#endif // _DEBUG
}

TLGraphicsEngine::ConstBuffer::~ConstBuffer()
{

}

void TLGraphicsEngine::ConstBuffer::Update(ShaderType type, CBBase* const cb)
{
	auto* deviceContext = GraphicsEngine::Instance()->GetDeviceContext();
	
	ID3D11Buffer* buffer = GetBuffer();

	if(cb == nullptr) return;

	deviceContext->UpdateSubresource(buffer, 0, nullptr, cb, 0, 0);
	switch (type)
	{
		case ShaderType::Pixel:
		{			
			deviceContext->PSSetConstantBuffers(m_BindPoint, m_BindCnt, &buffer);
			break;
		}
		case ShaderType::Vertex:
		{
			deviceContext->VSSetConstantBuffers(m_BindPoint, m_BindCnt, &buffer);
			break;
		}
		case ShaderType::Geometry:
		{
			deviceContext->GSSetConstantBuffers(m_BindPoint, m_BindCnt, &buffer);
			break;
		}
		case ShaderType::Hull:
		{
			deviceContext->HSSetConstantBuffers(m_BindPoint, m_BindCnt, &buffer);
			break;
		}
		case ShaderType::Domain:
		{
			deviceContext->DSSetConstantBuffers(m_BindPoint, m_BindCnt, &buffer);
			break;
		}
		case ShaderType::Compute:
		{
			deviceContext->CSSetConstantBuffers(m_BindPoint, m_BindCnt, &buffer);
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