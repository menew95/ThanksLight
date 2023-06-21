#include "pch.h"
#include "Level8ButtonTrigger.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "ResourceManager.h"
#include "InteractiveObject.h"

TLGameEngine::Level8ButtonTrigger::Level8ButtonTrigger()
{

}

TLGameEngine::Level8ButtonTrigger::~Level8ButtonTrigger()
{

}

void TLGameEngine::Level8ButtonTrigger::Awake()
{
	m_gameObject.lock()->GetComponent<BoxCollider>()->AddTriggerEvent(this);

	m_interactiveObject = SceneManager::Instance().FindObject(m_PrimitiveID)->GetComponent<InteractiveObject>();
	m_buttonMeshRenderer = SceneManager::Instance().FindObject(m_buttonObjectID)->GetComponent<MeshRenderer>();

	m_openEmissive = ResourceManager::Instance().GetMaterial(m_openEmissiveID.guid);
	m_closeEmissive = ResourceManager::Instance().GetMaterial(m_closeEmissiveID.guid);
}

void TLGameEngine::Level8ButtonTrigger::OnTriggerEnter(std::string name)
{
	if (name != m_playerID.fileID && !m_interactiveObject.lock()->GetIs2DObject())
	{
		if ((name == m_PrimitiveID.fileID && !m_interactiveObject.lock()->GetIs2DObject())
			|| name != m_PrimitiveID.fileID)
		{
			m_iNum++;
			m_IsTrigger = true;

			if (!m_buttonMeshRenderer.expired())
			{
				m_buttonMeshRenderer.lock()->SetMaterial(m_openEmissive, 2);
				m_buttonMeshRenderer.lock()->SetMaterial(m_openEmissive, 3);
			}
		}
	}
}

void TLGameEngine::Level8ButtonTrigger::OnTriggerExit(std::string name)
{
	if (name != m_playerID.fileID)
	{
		if ((name == m_PrimitiveID.fileID && !m_interactiveObject.lock()->GetIs2DObject())
			|| name != m_PrimitiveID.fileID)
		{
			m_iNum--;
			if (m_iNum != 0)
			{
				return;
			}
			m_IsTrigger = false;

			if (!m_buttonMeshRenderer.expired())
			{
				m_buttonMeshRenderer.lock()->SetMaterial(m_closeEmissive, 2);
				m_buttonMeshRenderer.lock()->SetMaterial(m_closeEmissive, 3);
			}
		}
	}
}
