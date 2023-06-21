#include "Pch.h"
#include "Engine.h"

#include "IGraphicsEngine.h"

#ifdef _DEBUG
	#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

Engine::~Engine()
{
	m_graphicsEngine->Finalize();
	m_graphicsDestructor(m_graphicsEngine);
	TLGameEngine::GameEngine::Release();
}

void Engine::Init(const WindowInfo& info)
{
	m_engine = &TLGameEngine::GameEngine::Instance();

#ifdef x64
#ifdef _DEBUG
	HMODULE graphicsDLL = LoadLibrary(_T("GraphicsEngine_Debug_x64.dll"));
#else
	HMODULE graphicsDLL = LoadLibrary(_T("GraphicsEngine_Release_x64.dll"));
#endif
#else
#ifdef _DEBUG
	HMODULE graphicsDLL = LoadLibrary(_T("GraphicsEngine_Debug_x86.dll"));
#else
	HMODULE graphicsDLL = LoadLibrary(_T("GraphicsEngine_Release_x86.dll"));
#endif
#endif
	assert(graphicsDLL != nullptr);

	using TLGraphicsEngine::GraphicsEngineConstructor;
	using TLGraphicsEngine::GraphicsEngineDestructor;

	GraphicsEngineConstructor graphicsConstructor = (GraphicsEngineConstructor)GetProcAddress(graphicsDLL, "GraphicsEngineInstance");
	m_graphicsDestructor = (GraphicsEngineDestructor)GetProcAddress(graphicsDLL, "GraphicsEngineRelease");

	m_graphicsEngine = graphicsConstructor(TLGraphicsEngine::eEngineType::DirectX11);

	m_engine->Initialize(info, m_graphicsEngine);
}

void Engine::Loop()
{
	m_engine->Loop();
}

void Engine::OnSize(UINT width, UINT height, WPARAM wParam)
{
	if (m_engine != nullptr)
		m_engine->OnResize(width, height, wParam);
}

void Engine::OnMouseMove(int buttonState, int x, int y, int middleOfSceenX, int middleOfSceenY)
{
	m_engine->OnMouseMove(buttonState, x, y, middleOfSceenX, middleOfSceenY);
}
