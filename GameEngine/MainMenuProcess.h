#pragma once

#include "Component.h"

namespace TLGameEngine
{
class MainMenuProcess : public Component
{
public:
	MainMenuProcess();

	virtual void Start() override;

private:
	Vector2 m_stageSelectOriginPosition;

	FileID m_StartButton;
	FileID m_SelectStageButton;
	FileID m_ExitButton;
	FileID m_StageSelectBoard;
	FileID m_StageSelectCloseButton;

	BOOST_DESCRIBE_CLASS(TLGameEngine::MainMenuProcess, (), (), (), (m_StartButton, m_SelectStageButton, m_ExitButton, m_StageSelectBoard, m_StageSelectCloseButton))
};
}
