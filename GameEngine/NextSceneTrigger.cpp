#include "pch.h"
#include "NextSceneTrigger.h"
#include "SceneManager.h"
#include "BoxCollider.h"
#include "MyTime.h"
#include "GameManager.h"
#include "Image.h"

TLGameEngine::NextSceneTrigger::NextSceneTrigger()
{

}

TLGameEngine::NextSceneTrigger::~NextSceneTrigger()
{

}

void TLGameEngine::NextSceneTrigger::Awake()
{
	m_gameObject.lock()->GetComponent<BoxCollider>()->AddTriggerEvent(this);
	m_fadeOut = SceneManager::Instance().FindObject(m_fadeOutID)->GetComponent<Image>();
}

void TLGameEngine::NextSceneTrigger::Update()
{
	if (m_bCallNextScene)
	{
		m_fTimer += Time::Instance().GetDeltaTime();

		FadeOut();

		if (m_fTimer > 3.0f)
		{
			SceneManager::Instance().ChangeScene(m_nextSceneID);
		}
	}

	if (m_bFadeIn)
	{
		m_fTimer -= Time::Instance().GetDeltaTime();
		FadeIn();
	}
}

void TLGameEngine::NextSceneTrigger::FadeOut()
{
	float alpha = m_fTimer / 2.f;

	m_fadeOut.lock()->SetColor(1, 1, 1, alpha);
}

void TLGameEngine::NextSceneTrigger::FadeIn()
{
	float alpha = m_fTimer / 2.f;

	if (alpha > 1.0f)
	{
		alpha = 1.0f;
	}

	if (alpha <= 0.f)
	{
		m_bFadeIn = false;
		m_fadeOut.lock()->SetColor(1, 1, 1, 0);
		GameManager::Instance().SetKeybordInput(true);
	}
	else
	{
		m_fadeOut.lock()->SetColor(1, 1, 1, alpha);
	}
}

void TLGameEngine::NextSceneTrigger::OnTriggerEnter(std::string name)
{
	m_bCallNextScene = true;
	GameManager::Instance().SetKeybordInput(false);
}

void TLGameEngine::NextSceneTrigger::OnTriggerExit(std::string name)
{
}
