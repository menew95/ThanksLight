#include "pch.h"
#include "ResourceBase.h"

TLGraphicsEngine::ResourceBase::ResourceBase(eResourceType type)
: m_eResourceType(type)
, m_name()
{

}

TLGraphicsEngine::ResourceBase::ResourceBase()
: m_eResourceType()
, m_name()
{

}

TLGraphicsEngine::ResourceBase::ResourceBase(eResourceType type, const char* name)
: m_eResourceType(type)
, m_name(string(name))
{

}
