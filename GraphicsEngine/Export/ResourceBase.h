#pragma once

#include <wrl.h>

#include "..\..\Common\Object.h"

using namespace Microsoft::WRL;

namespace TLGraphicsEngine
{
	enum class eResourceType
	{
		Mesh = 0,
		Material = 1,
		Texture = 2,
		Shader = 3,
		Buffer = 4
	};

	class ResourceBase :
		public Common::Object
	{
	public:
		ResourceBase();
		ResourceBase(eResourceType type);
		ResourceBase(eResourceType type, const char* name);
		virtual ~ResourceBase() {};

	public:
		eResourceType GetResourceType() {
			return m_eResourceType;
		}
		const char* GetName() {
			return m_name.c_str();
		}

		//virtual void Release() abstract;
	private:
		std::string m_name;
		eResourceType m_eResourceType;
	};
}