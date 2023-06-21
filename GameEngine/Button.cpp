#include "pch.h"
#include "Button.h"
#include "RectTransform.h"
#include "GameEngine.h"

TLGameEngine::Button::Button()
	: Component(Type::Action)
{
}

void TLGameEngine::Button::Awake()
{
	m_transform = GetComponent<RectTransform>();
}

void TLGameEngine::Button::Update()
{
	if (Input::Instance().GetKeyDown(VK_LBUTTON))
	{
		if (IsClicked())
		{
			if (onClick != nullptr)
			{
				onClick();
			}
		}
	}
}

bool TLGameEngine::Button::IsClicked()
{
	WindowInfo window = GameEngine::GetWindowInfo();

	POINT cursorPosition;
	GetCursorPos(&cursorPosition);
	ScreenToClient(window.hwnd, &cursorPosition);

	cursorPosition.x -= window.width * 0.5f;
	cursorPosition.y -= window.height * 0.5f;
	cursorPosition.y *= -1;

	Rect rect = m_transform->GetRect();

	if (rect.x <= cursorPosition.x && rect.x + rect.width >= cursorPosition.x
		&& rect.y <= cursorPosition.y && rect.y + rect.height >= cursorPosition.y)
	{
		return true;
	}

	return false;
}
