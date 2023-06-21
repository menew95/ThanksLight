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

		// ������ �ʱ�ȭ�Ѵ�.
		virtual long Initialize(const WindowInfo& info, TLGraphicsEngine::IGraphicsEngine* graphicEngine);

		// ���� ���� ����. ���� ������ ����Ǵ� �ݺ�����
		virtual void Loop();

		// ������ �����Ѵ�.
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