#pragma once

#include "Singleton.h"

namespace TLGameEngine {
#define vk_alpha(k) (k - 'a' + 0x41)
#define vk_num(k) (k - '0' + 0x30)
class Input : public Singleton<Input>
{
private:
	enum class KeyState
	{
		Idle, 
		Press,
		Down,
		Up,
	};
	enum
	{
		KeyCount = 256,
	};
public:
	void Init(HWND hwnd);
	void Update();
 	bool GetKey(short keyCode) { return m_keyState[keyCode] == KeyState::Press; }
	bool GetKeyDown(short keyCode) { return m_keyState[keyCode] == KeyState::Down; }
	bool GetKeyUp(short keyCode) { return m_keyState[keyCode] == KeyState::Up; }
private:
	std::array<KeyState, KeyCount> m_keyState = {};
	HWND m_hwnd = 0;

	Input() = default;
	friend Singleton;
};
}
