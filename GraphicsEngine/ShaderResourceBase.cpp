#include "pch.h"
#include "ShaderResourceBase.h"


TLGraphicsEngine::ShaderResourceBase::ShaderResourceBase(const char* name, int bindPoint, int cnt, RegisterType registerType, ResourceType resourceType)
:
ResourceBase(eResourceType::Buffer, name),
m_BindPoint(bindPoint),
m_BindCnt(cnt),
m_RegisterType(registerType),
m_ResoureceType(resourceType)
{

}

TLGraphicsEngine::ShaderResourceBase::~ShaderResourceBase()
{

}
