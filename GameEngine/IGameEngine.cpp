#include "Export/IGameEngine.h"
#include "GameEngine.h"


GAME_CLASS_DECLSPEC GameEngine::IGameEngine* GameEngine::GameEngineInstance()
{
	return new GameEngine;
}

GAME_CLASS_DECLSPEC void GameEngine::GameEngineRelease(IGameEngine* gameEngine)
{
	if (gameEngine != nullptr)
	{
		delete gameEngine;
		gameEngine = nullptr;
	}
}