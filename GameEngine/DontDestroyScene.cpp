#include "pch.h"
#include "DontDestroyScene.h"
// UI
#include "Canvas.h"
#include "Text.h"
#include "Image.h"
#include "RectTransform.h"
#include "DebugManager.h"
#include "PostProcess.h"
#include "Camera.h"
#include "GameManager.h"

TLGameEngine::DontDestroyScene::DontDestroyScene()
{
	GameManager::Instance();
}

TLGameEngine::DontDestroyScene::~DontDestroyScene()
{
	GameManager::Release();
}

void TLGameEngine::DontDestroyScene::Load()
{
#pragma region UI 오브젝트 하드 코딩
	{
				std::shared_ptr<DebugManager> _debug = std::make_shared<DebugManager>();
		
				// Canvas
				{
					std::shared_ptr<GameObject> _canvasObject = std::make_shared<GameObject>();
					std::shared_ptr<RectTransform> _rect = std::make_shared<RectTransform>();
					_canvasObject->AddComponent(_rect);
					_canvasObject->AddComponent(_debug);
					AddGameObject(_canvasObject);
				}
		#pragma region Debug 정보
				{
					// Text
					{
						std::shared_ptr<GameObject> _textObject = std::make_shared<GameObject>();
						std::shared_ptr<Text> _text = std::make_shared<Text>();
						_text->SetFontSize(20);
						_textObject->AddComponent(_text);
						std::shared_ptr<RectTransform> _rect = std::make_shared<RectTransform>();
						_rect->SetRect({ -400, 300, 0 , 0});
						_textObject->AddComponent(_rect);
						AddGameObject(_textObject);
						_debug->m_FrameInfo = _text;
					}
		
					// Text
					{
						std::shared_ptr<GameObject> _textObject = std::make_shared<GameObject>();
						std::shared_ptr<Text> _text = std::make_shared<Text>();
						_text->SetFontSize(20);
						_textObject->AddComponent(_text);
						std::shared_ptr<RectTransform> _rect = std::make_shared<RectTransform>();
						_rect->SetRect({ -400, 250, 0, 0});
						_textObject->AddComponent(_rect);
						AddGameObject(_textObject);
						_debug->m_CameraInfo = _text;
					}
		
					// Text
					{
						std::shared_ptr<GameObject> _textObject = std::make_shared<GameObject>();
						std::shared_ptr<Text> _text = std::make_shared<Text>();
						_text->SetFontSize(20);
						_textObject->AddComponent(_text);
						std::shared_ptr<RectTransform> _rect = std::make_shared<RectTransform>();
						_rect->SetRect({ -400, 200, 0, 0 });
						_textObject->AddComponent(_rect);
						AddGameObject(_textObject);
						_debug->m_SceneInfo = _text;
					}
		
					// Text
					{
						std::shared_ptr<GameObject> _textObject = std::make_shared<GameObject>();
						std::shared_ptr<Text> _text = std::make_shared<Text>();
						_text->SetFontSize(20);
						_textObject->AddComponent(_text);
						std::shared_ptr<RectTransform> _rect = std::make_shared<RectTransform>();
						_rect->SetRect({-400, 150, 0, 0});
						_textObject->AddComponent(_rect);
						AddGameObject(_textObject);
						_debug->m_PostProcessInfo = _text;
					}
		
					// Text
					{
						std::shared_ptr<GameObject> _textObject = std::make_shared<GameObject>();
						std::shared_ptr<Text> _text = std::make_shared<Text>();
						_text->SetFontSize(20);
						_textObject->AddComponent(_text);
						std::shared_ptr<RectTransform> _rect = std::make_shared<RectTransform>();
						_rect->SetRect({ -400, 100, 0 });
						_textObject->AddComponent(_rect);
						AddGameObject(_textObject);
						_debug->m_PostProcessInfo2 = _text;
					}
				}
#pragma endregion

	}
#pragma endregion
}
