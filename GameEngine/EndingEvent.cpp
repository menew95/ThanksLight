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
	m_narration->AddSubtileText("������ �������ϴ�.");
	m_narration->AddSubtileText("����� ������ �ذ��ϰ� ���ư� �� ������");
	m_narration->AddSubtileText("�׻� ���� ���߾��� �����̶�� ���� ���� ���ʽÿ�.");
	m_narration->AddSubtileText("������ ���� ���ɰ� ������ �ذ����� ���̰� �� ���Դϴ�.");
	m_narration->AddSubtileText("�׸��� �� ���� ���� �� ������ �ٸ��� ���� �� �ֽ��ϴ�.");
	m_narration->AddSubtileText("���� �鸸 �ٶ��� ���� �پ��� ���� ���� ���߽ʽÿ�.");
	m_narration->AddSubtileText("�� ���ο� ���� �ȳ��� ���Դϴ�.");
	m_narration->AddSubtileText("���� �����Ͻʽÿ�.");
	m_narration->AddSubtileText("�׸���, ���ư��ʽÿ�.");
	m_narration->AddSubtileText("����� �̿��� ����� ������");
	m_narration->AddSubtileText("Thanks, �ּ��� ������, �Ǽ��� ������, ����ȯ ������, ���������");
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
