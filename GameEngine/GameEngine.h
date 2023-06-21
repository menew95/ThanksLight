#pragma once

#include "..\Common\Singleton.h"

namespace TLGraphicsEngine
{
class IGraphicsEngine;
}

namespace TLGameEngine
{
	/// <summary>
	/// GameEngine Class
	/// </summary>
	
	class GameEngine : public Singleton<GameEngine>
	{
	public:
		~GameEngine();

		// 게임을 초기화한다.
		virtual long Initialize(const WindowInfo& info, TLGraphicsEngine::IGraphicsEngine* graphicEngine);

		// 메인 게임 루프. 실제 게임이 진행되는 반복구문
		virtual void Loop();

		// 게임을 정리한다.
		virtual void Finalize();

		void OnResize(UINT width, UINT height, WPARAM wParam);
		void OnMouseMove(int buttonState, int x, int y, int middleOfSceenX, int middleOfSceenY);

		std::function<void(int, int, int, int, int)> OnMouseMoveEvent;

		static const WindowInfo& GetWindowInfo() { return _window; }

		TLGraphicsEngine::IGraphicsEngine* GetGraphicsEngine() { return m_graphicsEngine; }

	private:
		void ShowDebugTitle();

	private:
		static WindowInfo _window;

		class ComponentManager* _componentManager;
		TLGraphicsEngine::IGraphicsEngine* m_graphicsEngine;
	};

}