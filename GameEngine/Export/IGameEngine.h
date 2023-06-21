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
	/// ���� ���� �������̽�
	/// </summary>

	class GAME_CLASS_DECLSPEC IGameEngine abstract
	{
	public:
		// ������ �ʱ�ȭ�Ѵ�.
		virtual long Initialize(void* hInstance) abstract;

		// ���� ���� ����. ���� ������ ����Ǵ� �ݺ�����
		virtual void Loop() abstract;

		// ������ �����Ѵ�.
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