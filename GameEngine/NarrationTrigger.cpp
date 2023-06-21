#include "pch.h"
#include "NarrationTrigger.h"
#include "Narration.h"
#include "BoxCollider.h"

TLGameEngine::NarrationTrigger::NarrationTrigger()
	: Component(Type::Action)
{
}

TLGameEngine::NarrationTrigger::~NarrationTrigger()
{
}

void TLGameEngine::NarrationTrigger::Awake()
{
	GetComponent<BoxCollider>()->AddTriggerEvent(this);
	m_narration = GetComponent<Narration>();

	if (m_AutoTriggerTime != 0)
	{
		m_isAutoTrigger = true;
	}
}

void TLGameEngine::NarrationTrigger::Update()
{
	if (m_isAutoTrigger)
	{
		m_AutoTriggerTime -= Time::Instance().GetDeltaTime();

		if (m_AutoTriggerTime <= 0)
		{
			m_narration->ReadLine(0);

			m_isAutoTrigger = false;
			m_narration->SetPlay(true);
		}
	}
}

void TLGameEngine::NarrationTrigger::OnTriggerEnter(std::string name)
{
	if (name == m_Player.fileID)
	{
		m_narration->ReadLine(m_index);
		m_narration->SetPlay(true);
		GetComponent<BoxCollider>()->SetIsActive(false);
	}
}

void TLGameEngine::NarrationTrigger::OnTriggerExit(std::string name)
{
}
