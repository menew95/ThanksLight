#include "pch.h"
#include "Image.h"
#include "Texture.h"
#include "GameEngine.h"
#include "IGraphicsEngine.h"
#include "RectTransform.h"
#include "ResourceManager.h"
#include "Canvas.h"

TLGameEngine::Image::Image()
: Component(Type::Rendering)
{

}

TLGameEngine::Image::~Image()
{

}

void TLGameEngine::Image::Awake()
{
	m_rectTransform = GetComponent<RectTransform>();
	m_texture = ResourceManager::Instance().GetTexture(m_Sprite.guid);
}

void TLGameEngine::Image::LateUpdate()
{
}

void TLGameEngine::Image::Render()
{
	// Todo :: Rect 제대로 계산 필요
	Rect rect = m_rectTransform->GetRect();

	Vector3 local = m_rectTransform->GetLocalPosition();

	rect.x += (float)Canvas::m_canvas->m_width * 0.5f;
	rect.y += (float)Canvas::m_canvas->m_height * 0.5f;

	rect.y = Canvas::m_canvas->m_height - (rect.y + rect.height);

	GameEngine::Instance().GetGraphicsEngine()->RegistImageUIRenderQueue(rect.x, rect.y, m_texture->GetITexture().get(), &m_Color.r, rect, static_cast<int>(m_eImageType), m_fillAmount);
}

void TLGameEngine::Image::SetColor(float r, float g, float b, float a)
{
	m_Color = { r, g, b, a };
}

