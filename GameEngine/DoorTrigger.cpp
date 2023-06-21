#include "pch.h"
#include "DoorTrigger.h"
#include "BoxCollider.h"
#include "GameObject.h"

TLGameEngine::DoorTrigger::DoorTrigger()
{

}

TLGameEngine::DoorTrigger::~DoorTrigger()
{

}

void TLGameEngine::DoorTrigger::Awake()
{
	m_gameObject.lock()->GetComponent<BoxCollider>()->AddTriggerEvent(this);
}

void TLGameEngine::DoorTrigger::OnTriggerEnter(std::string name)
{
	if (name == m_PlayerID.fileID)
	{
		m_IsTrigger = true;
	}

}

void TLGameEngine::DoorTrigger::OnTriggerExit(std::string name)
{
	if (name == m_PlayerID.fileID)
	{
		m_IsTrigger = false;
	}
}
