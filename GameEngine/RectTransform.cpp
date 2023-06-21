#include "pch.h"
#include "RectTransform.h"
#include "Canvas.h"

TLGameEngine::RectTransform::RectTransform()
{
	m_Pivot = DirectX::SimpleMath::Vector2::Zero;
}

TLGameEngine::RectTransform::~RectTransform()
{
}

void TLGameEngine::RectTransform::Start()
{
}

void TLGameEngine::RectTransform::Update()
{
	std::shared_ptr<RectTransform> parent = std::dynamic_pointer_cast<RectTransform>(m_Parent.lock());

	if (parent != nullptr)
	{
		Vector2 resolution = { parent->m_resolution.x, parent->m_resolution.y };
		m_rect.width = (m_AnchorMax.x - m_AnchorMin.x) * resolution.x + m_SizeDelta.x;
		m_rect.height = (m_AnchorMax.y - m_AnchorMin.y) * resolution.y + m_SizeDelta.y;
		m_rect.x = resolution.x * (m_AnchorMin.x - 0.5f);
		m_rect.y = resolution.y * (m_AnchorMin.y - 0.5f);

		m_rect.x += m_AnchoredPosition.x - m_SizeDelta.x * 0.5f + m_rect.width * (0.5f - m_Pivot.x);
		m_rect.y += m_AnchoredPosition.y - m_SizeDelta.y * 0.5f + m_rect.height * (0.5f - m_Pivot.y);

		m_rect.x += parent->m_rect.x + parent->m_SizeDelta.x * 0.5f + parent->m_rect.width * parent->m_Pivot.x;
		m_rect.y += parent->m_rect.y + parent->m_SizeDelta.y * 0.5f + parent->m_rect.height * parent->m_Pivot.y;
	}

	m_resolution.x = m_rect.width;
	m_resolution.y = m_rect.height;

	m_LocalPosition.x = m_rect.x + m_rect.width * 0.5f;
	m_LocalPosition.y = m_rect.y + m_rect.height * 0.5f;
}
