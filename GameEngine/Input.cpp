#include "pch.h"
#include "Input.h"

void TLGameEngine::Input::Init(HWND hwnd)
{
	m_hwnd = hwnd;
	memset(&m_keyState[0], static_cast<int>(KeyState::Idle), KeyCount);
}

void TLGameEngine::Input::Update()
{
	HWND hwnd = ::GetActiveWindow();

	if (hwnd != m_hwnd)
	{
		for (size_t i = 0; i < KeyCount; i++)
	m_keyState[i] = KeyState::Idle;
		return;
	}
	
	BYTE keyboardStates[KeyCount] = {};
	if (::GetKeyboardState(keyboardStates) == false)
		return;

	for (size_t i = 0; i < KeyCount; i++)
	{
		KeyState& state = m_keyState[i];

		if (keyboardStates[i] & 0x80)
		{
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Press;
			else
				state = KeyState::Down;
		}
		else
		{
			if (state == KeyState::Idle || state == KeyState::Up)
				state = KeyState::Idle;
			else
				state = KeyState::Up;
		}
	}
}