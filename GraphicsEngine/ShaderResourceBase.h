#pragma once

//#include "../Common/ResourceBase.h"
#include "Export/ResourceBase.h"

/// <summary>
/// Shader에 연결될 리소스들의 베이스
/// </summary>

namespace TLGraphicsEngine
{
	enum class ResourceType
	{
		Buffer = 0,
		Texture = 1,
		ETC = 2
	};
	
	enum class RegisterType
	{
		Texture,	// SRV
		Sampler,	// Sampler
		UAV,	// UAV
		ConstBuffer	// CBV
	};

	class ShaderResourceBase : public ResourceBase
	{
	public:
		ShaderResourceBase(const char* name, int slotNum, int cnt, RegisterType registerType, ResourceType resourceType);
		virtual ~ShaderResourceBase();

	public:
		ResourceType m_ResoureceType = ResourceType::Buffer;

		int m_BindPoint = 0;
		int m_BindCnt = 0;

		RegisterType m_RegisterType;
	public:
		//virtual void Release() abstract;
	};
}