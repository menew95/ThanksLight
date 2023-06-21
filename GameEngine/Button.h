#pragma once

#include "Component.h"

namespace TLGameEngine
{
class RectTransform;

class Button : public Component
{

public:
	Button();

public:
	std::function<void()> onClick;

public:
	virtual void Awake() override;
	virtual void Update() override;

private:
	bool IsClicked();

	std::shared_ptr<RectTransform> m_transform;
	BOOST_DESCRIBE_CLASS(TLGameEngine::Button, (), (), (), ())
};
}

