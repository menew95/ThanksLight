#include "pch.h"
#include "Level3Event.h"
#include "SceneManager.h"
#include "Narration.h"
#include "InteractiveObject.h"
#include "GameManager.h"

TLGameEngine::Level3Event::Level3Event()
	: Component(Type::Action)
{

}

TLGameEngine::Level3Event::~Level3Event()
{

}

void TLGameEngine::Level3Event::Awake()
{
	m_narration1 = SceneManager::Instance().FindObject(m_narration1ID)->GetComponent<Narration>();
	m_narration2 = SceneManager::Instance().FindObject(m_narration2ID)->GetComponent<Narration>();
	m_interativeObject = SceneManager::Instance().FindObject(m_interativeObjectID)->GetComponent<InteractiveObject>();
}

void TLGameEngine::Level3Event::Update()
{
	if (m_event != m_maxEvent)
	{
		if (m_narration1.lock() != nullptr && m_narration1.lock()->GetIsPlaying())
		{
			auto _currNarration = m_narration1.lock()->GetLine();

			if (_currNarration == 1 && m_event == 0)
			{
				m_event++;
				GameManager::Instance().SetKeybordInput(false);
			}
		}

		if (!m_narration1.lock()->GetIsPlaying() && m_event == 1)
		{
			m_event++;
			GameManager::Instance().SetKeybordInput(true);
		}
	}
	else
	{
		if (!m_interativeObject.lock()->GetIs2DObject())
		{
			m_narration2.lock()->SetPlay(true);
		}
	}
}
