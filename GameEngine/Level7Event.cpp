#include "pch.h"
#include "Level7Event.h"
#include "SceneManager.h"
#include "Narration.h"
#include "Door.h"
#include "GameManager.h"
#include "Rigidbody.h"
TLGameEngine::Level7Event::Level7Event()
	:Component(Type::Action)
{

}

TLGameEngine::Level7Event::~Level7Event()
{

}

void TLGameEngine::Level7Event::Awake()
{
	m_door = SceneManager::Instance().FindObject(m_doorID)->GetComponent<Door>();
	m_narration1 = SceneManager::Instance().FindObject(m_narrationID)->GetComponent<Narration>();
	m_narration2 = GetComponent<Narration>();
	m_interactive = SceneManager::Instance().FindObject(m_interactiveID)->GetComponent<Rigidbody>();

#ifdef _DEBUG
	assert(m_door.lock() != nullptr || m_narration1.lock() != nullptr || m_narration2.lock() != nullptr);
#endif // _DEBUG

}

void TLGameEngine::Level7Event::Update()
{
	if(m_bEndEvnet)
	{
		return;
	}

	if (m_narration1.lock()->GetIsPlaying() && m_event == 0)
	{
		m_event++;
		GameManager::Instance().SetKeybordInput(false);
	}

	if (m_narration1.lock()->GetLine() == 3 && m_event == 1)
	{
		m_event++;
		m_interactive.lock()->SetGravity(true);
		GameManager::Instance().SetKeybordInput(true);
	}

	if (m_door.lock()->GetDoorOpen())
	{
		if (!m_narration2.lock()->GetIsPlaying())
		{
			m_narration2.lock()->ReadLine(0);
			m_bEndEvnet = true;
		}
	}
}
