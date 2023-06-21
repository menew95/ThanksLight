#pragma once
#include "Component.h"

namespace TLGameEngine
{
class Texture;
class RectTransform;

enum class eImageType
{
	Simple = 0,
	Filled = 1
};

class Image :
    public Component
{
public:
	Image();
	~Image();

	virtual void Awake() override;
	virtual void LateUpdate() override;

	void SetTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

	void Render();
	void SetColor(float r, float g, float b, float a);
protected:
	std::shared_ptr<RectTransform> m_rectTransform;
	std::shared_ptr<Texture> m_texture;

	ParsingColor m_Color;
	eImageType m_eImageType;
	float m_fillAmount;
	FileInfo m_Sprite;

	BOOST_DESCRIBE_CLASS(TLGameEngine::Image, (), (), (m_Color, m_Sprite), ())
};
}