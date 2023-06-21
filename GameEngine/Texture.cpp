#include "pch.h"
#include "Texture.h"

void TLGameEngine::Texture::SetITexture(std::shared_ptr<TLGraphicsEngine::ITexture> texture)
{
	m_iTexture = texture;
}
