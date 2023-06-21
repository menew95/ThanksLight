#pragma once

#include "Object.h"

namespace TLGraphicsEngine
{
	class ITexture;
}

namespace TLGameEngine
{
class Texture : public Common::Object
{
public:
	virtual ~Texture() {}

	void SetITexture(std::shared_ptr<TLGraphicsEngine::ITexture> texture);

	std::shared_ptr<TLGraphicsEngine::ITexture> GetITexture() { return m_iTexture; }

private:
	std::shared_ptr<TLGraphicsEngine::ITexture> m_iTexture;

};
}
