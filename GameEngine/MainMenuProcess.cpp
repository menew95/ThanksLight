#include "pch.h"
#include "MainMenuProcess.h"
#include "Button.h"
#include "SceneManager.h"
#include "SceneLoader.h"
#include "RectTransform.h"

TLGameEngine::MainMenuProcess::MainMenuProcess()
	: Component(Type::Action)
{
}

void TLGameEngine::MainMenuProcess::Start()
{
	CURSORINFO cursorInfo = { 0 };
	cursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursorInfo);
	if (cursorInfo.flags == 0)
	{
		ShowCursor(true);
	}

	{
		std::shared_ptr<GameObject> startButtonGameObject = SceneManager::Instance().FindObject(m_StartButton);
		std::shared_ptr<Button> startButton = startButtonGameObject->GetComponent<Button>();

		startButton->onClick = [startButtonGameObject]() {
			std::shared_ptr<SceneLoader> sceneLoader = startButtonGameObject->GetComponent<SceneLoader>();
			SceneManager::Instance().LoadScene(sceneLoader->GetLoadSceneIndex());
			SceneManager::Instance().UnloadScene(0);
		};
	}

	{
		std::shared_ptr<GameObject> stageSelectBoardGameObject = SceneManager::Instance().FindObject(m_StageSelectBoard);
		std::shared_ptr<RectTransform> stageSelectTransform = stageSelectBoardGameObject->GetComponent<RectTransform>();
		m_stageSelectOriginPosition = stageSelectTransform->GetAnchoredPosition();
		stageSelectTransform->SetAnchoredPosition({ 9999, 9999 });
		
		std::shared_ptr<GameObject> stageSelectButtonGameObject = SceneManager::Instance().FindObject(m_SelectStageButton);
		std::shared_ptr<Button> stageSelectButton = stageSelectButtonGameObject->GetComponent<Button>();
		stageSelectButton->onClick = [stageSelectTransform, this]() {
			stageSelectTransform->SetAnchoredPosition(m_stageSelectOriginPosition);
		};

		std::shared_ptr<GameObject> closeButtonGameObject = SceneManager::Instance().FindObject(m_StageSelectCloseButton);
		std::shared_ptr<Button> closeButton = closeButtonGameObject->GetComponent<Button>();
		closeButton->onClick = [stageSelectTransform, this]() {
			stageSelectTransform->SetAnchoredPosition({ 9999, 9999 });
		};
	}

	{
		std::shared_ptr<GameObject> exitButtonGameObject = SceneManager::Instance().FindObject(m_ExitButton);
		std::shared_ptr<Button> exitButton = exitButtonGameObject->GetComponent<Button>();

		exitButton->onClick = []() {

			PostQuitMessage(0);
		};
	}
}
