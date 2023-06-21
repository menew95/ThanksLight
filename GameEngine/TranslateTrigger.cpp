#include "pch.h"
#include "TranslateTrigger.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"

TLGameEngine::TranslateTrigger::TranslateTrigger()
{

}

TLGameEngine::TranslateTrigger::~TranslateTrigger()
{
}

void TLGameEngine::TranslateTrigger::Awake()
{
	m_gameObject.lock()->GetComponent<BoxCollider>()->AddTriggerEvent(this);
}

void TLGameEngine::TranslateTrigger::Update()
{
	while (!m_TranslateObjects.empty())
	{
		Vector3 teleportPos = { m_worldTranslatePos_X, m_worldTranslatePos_Y, m_worldTranslatePos_Z };

		m_TranslateObjects.front().lock()->SetWorldPosition(teleportPos);

		m_TranslateObjects.pop();
	}
}

void TLGameEngine::TranslateTrigger::OnTriggerEnter(std::string name)
{
	auto obj = SceneManager::Instance().FindObject(name);
	
	if (obj)
	{
		m_TranslateObjects.push(obj->GetComponent<Transform>());
	}
}
