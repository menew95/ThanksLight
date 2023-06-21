#include "pch.h"
#include "TableSnapTrigger.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "InteractiveObject.h"
#include "SceneManager.h"
#include "Transform.h"
#include "Rigidbody.h"

TLGameEngine::TableSnapTrigger::TableSnapTrigger()
{

}

TLGameEngine::TableSnapTrigger::~TableSnapTrigger()
{

}

void TLGameEngine::TableSnapTrigger::Awake()
{
	m_gameObject.lock()->GetComponent<BoxCollider>()->AddTriggerEvent(this);
	m_interactive = SceneManager::Instance().FindObject(m_interactiveID)->GetComponent<InteractiveObject>();
	m_interactiveTransform = m_interactive.lock()->GetTransform();
}

void TLGameEngine::TableSnapTrigger::Update()
{
	m_bIsSnap = false;
	if (m_bOnOff == false)
	{
		return;
	}
	if (m_IsTrigger && !m_interactive.lock()->GetIsGrabed()
		&& !m_interactive.lock()->GetIs2DObject())
	{
		// 그랩 상태가 아니고 3D 물체일때
		SnapObject();
	}
}

void TLGameEngine::TableSnapTrigger::OnTriggerEnter(std::string name)
{
	if (name == m_interactiveID.fileID)
	{
		m_IsTrigger = true;
	}
}

void TLGameEngine::TableSnapTrigger::OnTriggerExit(std::string name)
{
	if (name == m_interactiveID.fileID)
	{
		m_IsTrigger = false;
	}
}

void TLGameEngine::TableSnapTrigger::SnapObject()
{
	m_bIsSnap = true;

	auto currPos = m_interactiveTransform.lock()->GetLocalPosition();

	currPos.x = 0;
	currPos.y = 1.2;
	m_interactiveTransform.lock()->SetLocalPosition(currPos);
	auto zero = DirectX::SimpleMath::Vector3::Zero;
	m_interactiveTransform.lock()->SetLocalRotate(zero);
	m_interactiveTransform.lock()->GetComponent<Rigidbody>()->SetGravity(false);
}
