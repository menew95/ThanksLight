#include "pch.h"
#include "Level5ButtonTrigger.h"
#include "BoxCollider.h"
#include "InteractiveObject.h"
#include "SceneManager.h"

TLGameEngine::Level5ButtonTrigger::Level5ButtonTrigger():
	Component(Component::Type::Action)
{

}

TLGameEngine::Level5ButtonTrigger::~Level5ButtonTrigger()
{

}

void TLGameEngine::Level5ButtonTrigger::Awake()
{
	m_Button = GetComponent<InteractiveObject>();
	m_Collider = SceneManager::Instance().FindObject(m_ButtonTrigger)->GetComponent<BoxCollider>();
}

void TLGameEngine::Level5ButtonTrigger::Update()
{
	if (!m_Button.lock()->GetIs2DObject())
	{
		m_Collider.lock()->SetIsActive(true);
	}
	else
	{
		m_Collider.lock()->SetIsActive(false);
	}
}
