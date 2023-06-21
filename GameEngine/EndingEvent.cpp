#include "pch.h"
#include "EndingEvent.h"
#include "SceneManager.h"
#include "RectTransform.h"
#include "GameObject.h"
#include "Text.h"
#include "Narration.h"

TLGameEngine::EndingEvent::EndingEvent()
	: Component(Type::Action)
{
}

void TLGameEngine::EndingEvent::Awake()
{
	SceneManager& sceneManager = SceneManager::Instance();
	for (auto imageID : m_Images)
	{
		std::shared_ptr<GameObject> imageGameObject = sceneManager.FindObject(imageID);
		m_images.push_back(imageGameObject);

		imageGameObject->GetComponent<RectTransform>()->SetAnchoredPosition({ 9999, 9999 });
	}
	m_currentTime = m_ImageTransitionIntervalTime;

	m_narration = sceneManager.FindObject(m_Narration)->GetComponent<Narration>();

	std::string text = "";
	m_narration->AddSubtileText("실험이 끝났습니다.");
	m_narration->AddSubtileText("당신이 문제를 해결하고 나아갈 수 있음에");
	m_narration->AddSubtileText("항상 빛이 비추었기 때문이라는 것을 잊지 마십시오.");
	m_narration->AddSubtileText("문제에 대한 관심과 관찰은 해결점을 보이게 할 것입니다.");
	m_narration->AddSubtileText("그리고 그 빛에 따라 그 문제가 다르게 보일 수 있습니다.");
	m_narration->AddSubtileText("한쪽 면만 바라보지 말고 다양한 곳에 빛을 비추십시오.");
	m_narration->AddSubtileText("더 새로운 길을 안내할 것입니다.");
	m_narration->AddSubtileText("빛에 감사하십시오.");
	m_narration->AddSubtileText("그리고, 나아가십시오.");
	m_narration->AddSubtileText("오경민 이원택 정희범 조영래");
	m_narration->AddSubtileText("Thanks, 최성웅 교수님, 권순형 교수님, 김주환 교수님, 게임인재원");
	m_narration->AddSubtileText("Using Barking Dog,BasicShapesCollection, BrickProjectStudio, MASH Virtual, Naver Clova Dubbing");
}

void TLGameEngine::EndingEvent::Update()
{
	if (m_currentTime >= m_ImageTransitionIntervalTime && m_currentIndex < m_images.size())
	{
		m_images[m_currentIndex]->GetComponent<RectTransform>()->SetAnchoredPosition({ 0, 0 });
		m_currentTime -= m_ImageTransitionIntervalTime;
		m_currentIndex++;
	}

	m_currentTime += Time::Instance().GetDeltaTime();

	if (Input::Instance().GetKeyDown(VK_ESCAPE))
	{
		SceneManager::Instance().UnloadScene(SceneManager::Instance().GetCurrentSceneIndex());
		SceneManager::Instance().LoadScene(0);
	}
}
