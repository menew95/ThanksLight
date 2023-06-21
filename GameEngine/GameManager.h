#pragma once
#include "..\Common\Singleton.h"

namespace TLGameEngine
{
	class GameManager : public Singleton<GameManager>
	{
	public:
		GameManager();
		~GameManager();

	public:
		void SetKeybordInput(bool value) { m_keybordInput = value; }
		bool GetKeybordInput() { return m_keybordInput; }

		void SetPause(bool value) { m_gamePause = value; }
		bool GetPause() { return m_gamePause; }

		void SetCanConvert2D(bool value) { m_canConvert2D = value; }
		bool GetCanConvert2D() { return m_canConvert2D; }

		void SetCanConvert3D(bool value) { m_canConvert3D = value; }
		bool GetCanConvert3D() { return m_canConvert3D; }

		void InitSetting();

	private:
		bool m_keybordInput = true;
		bool m_canConvert2D = true;
		bool m_canConvert3D = true;
		bool m_gamePause = false;

	};
}

