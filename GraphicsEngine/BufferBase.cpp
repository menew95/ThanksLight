#include "pch.h"
#include "BufferBase.h"
#include "GraphicsEngine.h"


TLGraphicsEngine::BufferBase::BufferBase(const char* name, int bindPoint, int bindCnt, int size, RegisterType type)
:
ShaderResourceBase(name, bindPoint, bindCnt, type, ResourceType::Buffer),
m_Size(size)
{

}

int TLGraphicsEngine::BufferBase::GetSize()
{
	return m_Size;
}

ID3D11Buffer* TLGraphicsEngine::BufferBase::GetBuffer()
{
	return m_Buffer.Get();
}

ID3D11Buffer* const* TLGraphicsEngine::BufferBase::GetBufferRef()
{
	return m_Buffer.GetAddressOf();
}
