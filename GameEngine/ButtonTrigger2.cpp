#include "pch.h"
#include "ButtonTrigger2.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"

#include "MeshRenderer.h"
#include "MeshFilter.h"
#include "Material.h"
#include "ResourceManager.h"

#include "InteractiveObject.h"

TLGameEngine::ButtonTrigger2::ButtonTrigger2()
{

}

TLGameEngine::ButtonTrigger2::~ButtonTrigger2()
{

}

void TLGameEngine::ButtonTrigger2::Awake()
{
	m_gameObject.lock()->GetComponent<BoxCollider>()->AddTriggerEvent(this);

	m_buttonMeshRenderer = SceneManager::Instance().FindObject(m_buttonObjectID)->GetComponent<MeshRenderer>();

	m_openEmissive = ResourceManager::Instance().GetMaterial(m_openEmissiveID.guid);
	m_closeEmissive = ResourceManager::Instance().GetMaterial(m_closeEmissiveID.guid);

	m_defaultState = m_SetState;
}

void TLGameEngine::ButtonTrigger2::Update()
{
	m_IsTrigger = m_defaultState;

	while (!m_CurrentTriggerEnterObject.empty())
	{
		if (m_CurrentTriggerEnterObject.front())
		{
			m_IsTrigger = !m_defaultState;
		}
		m_CurrentTriggerEnterObject.pop();
	}

	if (m_IsTrigger)
	{
		if (!m_buttonMeshRenderer.expired())
		{
			m_buttonMeshRenderer.lock()->SetMaterial(m_openEmissive, 2);
			m_buttonMeshRenderer.lock()->SetMaterial(m_openEmissive, 3);
		}
	}
	else
	{
		if (!m_buttonMeshRenderer.expired())
		{
			m_buttonMeshRenderer.lock()->SetMaterial(m_closeEmissive, 2);
			m_buttonMeshRenderer.lock()->SetMaterial(m_closeEmissive, 3);
		}
	}
}

void TLGameEngine::ButtonTrigger2::OnTriggerEnter(std::string name)
{
}

void TLGameEngine::ButtonTrigger2::OnTriggerPersist(std::string name)
{
	auto enterObject = SceneManager::Instance().FindObject(name);

	if (enterObject)
	{
		auto interactiveComp = enterObject->GetComponent<InteractiveObject>();

		if (interactiveComp)
		{

			if (!interactiveComp->GetIs2DObject())
			{

				auto KeyObject = interactiveComp->Get3DObjects();
				auto KeyObjectID = KeyObject->GetComponent<MeshFilter>();

				bool result = (KeyObjectID->GetMeshUID() == m_KeyObject.guid);
				m_CurrentTriggerEnterObject.push(result);
			}
		}
	}
}

void TLGameEngine::ButtonTrigger2::OnTriggerExit(std::string name)
{


}
