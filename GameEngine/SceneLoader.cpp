#include "pch.h"
#include "SceneLoader.h"
#include "SceneManager.h"
#include "Button.h"

TLGameEngine::SceneLoader::SceneLoader()
	: Component(Type::Action)
{
}

void TLGameEngine::SceneLoader::Awake()
{
	std::shared_ptr<Button> button = GetComponent<Button>();

	if (button != nullptr)
	{
		button->onClick = [this]() {
			SceneManager::Instance().LoadScene(m_LoadSceneIndex);
			SceneManager::Instance().UnloadScene(0);
		};
	}
}

void TLGameEngine::SceneLoader::Update()
{
}
