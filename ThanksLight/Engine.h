#pragma once

#include "GameEngine.h"

class Scene;

class Engine
{

public:
	~Engine();

	void Init(const WindowInfo& info);
	void Loop();

	void OnSize(UINT width, UINT height, WPARAM wParam);

	void OnMouseMove(int buttonState, int x, int y, int middleOfSceenX, int middleOfSceenY);

private:
	TLGameEngine::GameEngine* m_engine;
	TLGraphicsEngine::IGraphicsEngine* m_graphicsEngine;
	void (*m_graphicsDestructor)(TLGraphicsEngine::IGraphicsEngine*);
};

