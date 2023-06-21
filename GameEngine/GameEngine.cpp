#include "pch.h"
#include "GameEngine.h"
#include "MyTime.h"
#include "Input.h"
#include "ComponentManager.h"
#include "PhysicsManager.h"
#include "ResourceManager.h"
#include "IGraphicsEngine.h"
#include "SceneManager.h"
#include "Factory.h"
#include "AudioSource.h"

#include <strsafe.h>

WindowInfo TLGameEngine::GameEngine::_window;

TLGameEngine::GameEngine::~GameEngine()
{
	Finalize();
}

long TLGameEngine::GameEngine::Initialize(const WindowInfo& info, TLGraphicsEngine::IGraphicsEngine* graphicEngine)
{
	_window = info;

	Time::Instance().Init();
	Input::Instance().Init(info.hwnd);

	//RECT nowRect;
	//DWORD _style = (DWORD)GetWindowLong(info.hwnd, GWL_STYLE);
	//DWORD _exstyle = (DWORD)GetWindowLong(info.hwnd, GWL_EXSTYLE);

	//GetWindowRect(info.hwnd, &nowRect);

	//RECT newRect;
	//newRect.left = 0;
	//newRect.top = 0;
	//newRect.right = info.width;
	//newRect.bottom = info.height;

	//AdjustWindowRectEx(&newRect, _style, NULL, _exstyle);

	//// 클라이언트 영역보다 윈도 크기는 더 커야 한다. (외곽선, 타이틀 등)
	//int _newWidth = (newRect.right - newRect.left);
	//int _newHeight = (newRect.bottom - newRect.top);

	//SetWindowPos(info.hwnd, HWND_NOTOPMOST, nowRect.left, nowRect.top,
	//	_newWidth, _newHeight, SWP_SHOWWINDOW);

	m_graphicsEngine = graphicEngine;

	m_graphicsEngine->InitializeGraphic(nullptr, info.hwnd, info.width, info.height);

	_componentManager = &ComponentManager::Instance();
	ResourceManager::Instance().Init();

	Factory::Init();

	SceneManager::Instance().LoadScene(0);
	SceneManager::Instance().LoadDontDestroyScene();
	PhysicsManager::Instance();

	AudioSource::Init();

	return S_OK;
}

void TLGameEngine::GameEngine::Loop()
{
	Time::Instance().Update();
	Input::Instance().Update();
	ShowDebugTitle();
	
	_componentManager->Start();
	_componentManager->Update();

	m_graphicsEngine->BeginRender();
	m_graphicsEngine->Render();
	m_graphicsEngine->EndRender();

	_componentManager->Destroy();
	SceneManager::Instance().Clear();
}

void TLGameEngine::GameEngine::Finalize()
{
	Time::Instance().Release();
	Input::Instance().Release();
	SceneManager::Instance().Release();
	ComponentManager::Instance().Release();
	ResourceManager::Instance().Release();
	PhysicsManager::Instance().Release();
	AudioSource::Release();
}

void TLGameEngine::GameEngine::OnResize(UINT width, UINT height, WPARAM wParam)
{
	_window.width = width;
	_window.height = height;

	m_graphicsEngine->SetResize(width, height, wParam);
}

void TLGameEngine::GameEngine::OnMouseMove(int buttonState, int x, int y, int middleOfSceenX, int middleOfSceenY)
{
	if (OnMouseMoveEvent != nullptr)
	{
		OnMouseMoveEvent(buttonState, x, y, middleOfSceenX, middleOfSceenY);
	}
}

void TLGameEngine::GameEngine::ShowDebugTitle()
{
	size_t fps = Time::Instance().GetFps();
	if (fps == 0) return;

	WCHAR text[100] = L"";
	StringCbPrintf(text, 100, L"Thanks Light !  \tFPS : % d ms : % f  ", fps, 1000.f / fps);

	SetWindowText(_window.hwnd, text);

	if (Input::Instance().GetKeyDown(VK_F1))
	{
		m_graphicsEngine->OnOffDebug();
	}
	if (Input::Instance().GetKeyDown(VK_F2))
	{
		m_graphicsEngine->OnDebug(0);
	}
	if (Input::Instance().GetKeyDown(VK_F3))
	{
		m_graphicsEngine->OnDebug(1);
	}
	if (Input::Instance().GetKeyDown(VK_F4))
	{
		m_graphicsEngine->OnDebug(2);
	}
}