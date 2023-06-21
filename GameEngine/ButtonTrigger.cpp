#include "pch.h"
#include "ButtonTrigger.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "ResourceManager.h"
#include "InteractiveObject.h"

TLGameEngine::ButtonTrigger::ButtonTrigger()
{

}

TLGameEngine::ButtonTrigger::~ButtonTrigger()
{

}

void TLGameEngine::ButtonTrigger::Awake()
{
	m_gameObject.lock()->GetComponent<BoxCollider>()->AddTriggerEvent(this);

	m_interactiveObject = SceneManager::Instance().FindObject(m_PrimitiveID)->GetComponent<InteractiveObject>();
	m_buttonMeshRenderer = SceneManager::Instance().FindObject(m_buttonObjectID)->GetComponent<MeshRenderer>();

	m_openEmissive = ResourceManager::Instance().GetMaterial(m_openEmissiveID.guid);
	m_closeEmissive = ResourceManager::Instance().GetMaterial(m_closeEmissiveID.guid);

}


void TLGameEngine::ButtonTrigger::Start()
{

}

void TLGameEngine::ButtonTrigger::OnTriggerEnter(std::string name)
{
	if (name == m_PrimitiveID.fileID && !m_interactiveObject.lock()->GetIs2DObject())
	{
		m_IsTrigger = true;
		
		if (!m_buttonMeshRenderer.expired())
		{
			m_buttonMeshRenderer.lock()->SetMaterial(m_openEmissive, 2);
			m_buttonMeshRenderer.lock()->SetMaterial(m_openEmissive, 3);
		}
	}
}



void TLGameEngine::ButtonTrigger::OnTriggerExit(std::string name)
{
	if (name == m_PrimitiveID.fileID)
	{
		m_IsTrigger = false;

		if (!m_buttonMeshRenderer.expired())
		{
			m_buttonMeshRenderer.lock()->SetMaterial(m_closeEmissive, 2);
			m_buttonMeshRenderer.lock()->SetMaterial(m_closeEmissive, 3);
		}
	}
}

void TLGameEngine::ButtonTrigger::SetForceTriggerEvent(bool value)
{
	m_IsTrigger = value;

	if (!m_buttonMeshRenderer.expired())
	{
		if (value)
		{
			m_buttonMeshRenderer.lock()->SetMaterial(m_openEmissive, 2);
			m_buttonMeshRenderer.lock()->SetMaterial(m_openEmissive, 3);
		}
		else
		{
			m_buttonMeshRenderer.lock()->SetMaterial(m_closeEmissive, 2);
			m_buttonMeshRenderer.lock()->SetMaterial(m_closeEmissive, 3);
		}
	}
}
