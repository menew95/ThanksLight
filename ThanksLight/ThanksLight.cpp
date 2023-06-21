// ThanksLight.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "Pch.h"
#include "framework.h"
#include "ThanksLight.h"
#include "Engine.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

Engine* g_game;
WindowInfo g_windowInfo;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_THANKSLIGHT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THANKSLIGHT));

	MSG msg;

	RECT rect;
	GetClientRect(g_windowInfo.hwnd, &rect);
	g_windowInfo.width = rect.right - rect.left;
	g_windowInfo.height = rect.bottom - rect.top;
	g_windowInfo.windowed = true;

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(119439);
#endif // _DEBUG

	g_game = new Engine();
	g_game->Init(g_windowInfo);

	ShowWindow(g_windowInfo.hwnd, nCmdShow);
	UpdateWindow(g_windowInfo.hwnd);

	// 기본 메시지 루프입니다:
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			g_game->Loop();
		}
	}

	delete g_game;
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THANKSLIGHT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//A
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1600, 900, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	g_windowInfo.hwnd = hWnd;
	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEMOVE:
	{
		RECT clidentRC;
		POINT p1, p2;

		GetClientRect(hWnd, &clidentRC);

		p1.x = clidentRC.left;
		p1.y = clidentRC.top;
		p2.x = clidentRC.right;
		p2.y = clidentRC.bottom;

		ClientToScreen(hWnd, &p1);
		ClientToScreen(hWnd, &p2);

		clidentRC.left = p1.x;
		clidentRC.top = p1.y;
		clidentRC.right = p2.x;
		clidentRC.bottom = p2.y;

		int middleOfScrrenX = (clidentRC.left + clidentRC.right) / 2.0f;
		int middleOfScrrenY = (clidentRC.top + clidentRC.bottom) / 2.0f;

		POINT screenPos;
		screenPos.x = LOWORD(lParam);
		screenPos.y = HIWORD(lParam);

		ClientToScreen(hWnd, &screenPos);

		g_game->OnMouseMove(wParam, screenPos.x, screenPos.y, middleOfScrrenX, middleOfScrrenY);


		return 0;
	}
	case WM_SIZE:
	{
		//마우스 화면내 움직이기 고정
		{
		RECT clidentRC;
		POINT p1, p2;

		GetClientRect(hWnd, &clidentRC);

		p1.x = clidentRC.left;
		p1.y = clidentRC.top;
		p2.x = clidentRC.right;
		p2.y = clidentRC.bottom;

		ClientToScreen(hWnd, &p1);
		ClientToScreen(hWnd, &p2);

		clidentRC.left = p1.x;
		clidentRC.top = p1.y;
		clidentRC.right = p2.x;
		clidentRC.bottom = p2.y;

		ClipCursor(&clidentRC);

		g_windowInfo.width = LOWORD(lParam);
		g_windowInfo.height = HIWORD(lParam);
		}

		// Save the new client area dimensions.
		g_game->OnSize(g_windowInfo.width, g_windowInfo.height, wParam);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
