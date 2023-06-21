#pragma once

#ifdef GAMEENGINE_EXPORTING
#define GAME_CLASS_DECLSPEC    __declspec(dllexport)
#define GAME_DLL_TEMPLATE
#else
#define GAME_CLASS_DECLSPEC    __declspec(dllimport)
#define GAME_DLL_TEMPLATE extern
#endif

namespace GameEngine
{
	/// <summary>
	/// 게임 엔진 인터페이스
	/// </summary>

	class GAME_CLASS_DECLSPEC IGameEngine abstract
	{
	public:
		// 게임을 초기화한다.
		virtual long Initialize(void* hInstance) abstract;

		// 메인 게임 루프. 실제 게임이 진행되는 반복구문
		virtual void Loop() abstract;

		// 게임을 정리한다.
		virtual void Finalize() abstract;
	};

	extern "C"
	{
		GAME_CLASS_DECLSPEC IGameEngine* GameEngineInstance();
		GAME_CLASS_DECLSPEC void GameEngineRelease(IGameEngine*);
		using GameEngineConstructor = IGameEngine * (*)();
		using GameEngineDestructor = void (*)(IGameEngine*);
	}
}