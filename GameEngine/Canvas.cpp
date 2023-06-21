#include "pch.h"
#include "Canvas.h"
#include "GameEngine.h"
#include "RectTransform.h"
#include "Image.h"

TLGameEngine::Canvas* TLGameEngine::Canvas::m_canvas = nullptr;

TLGameEngine::Canvas::Canvas()
	: Component(Type::Action)
{
	if (m_canvas != nullptr)
	{
	}
}

TLGameEngine::Canvas::~Canvas()
{
	if (m_canvas != nullptr)
	{
	}
}

void TLGameEngine::Canvas::Update()
{
	m_canvas = this;
	auto windowInfo = GameEngine::GetWindowInfo();

	m_width = windowInfo.width;
	m_height = windowInfo.height;
	
	std::shared_ptr<RectTransform> rectTransform = GetComponent<RectTransform>();
	
	assert(rectTransform != nullptr);
	rectTransform->SetResolution({ static_cast<float>(m_width), static_cast<float>(m_height) });

	auto& children = rectTransform->GetChildren();

	std::function<void(const std::shared_ptr<Transform>)> renderImageFunction;

	renderImageFunction = [&renderImageFunction](std::shared_ptr<Transform> transform) {
		std::shared_ptr<Image> image = transform->GetComponent<Image>();
		if (image != nullptr)
		{
			image->Render();
		}
		auto children = transform->GetChildren();
		for (auto child : children)
		{
			renderImageFunction(child);
		}
	};

	renderImageFunction(rectTransform);
}