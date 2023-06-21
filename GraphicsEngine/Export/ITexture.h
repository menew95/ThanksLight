#pragma once

#include "Export/ResourceBase.h"

namespace TLGraphicsEngine
{
	class ITexture :
		public ResourceBase
	{
	public:
		ITexture() {}
		ITexture(const char* name) : ResourceBase(eResourceType::Texture, name) {}

		virtual ~ITexture() {}
	};
}