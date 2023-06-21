#include "pch.h"
#include "Level8Event.h"
#include "SceneManager.h"
#include "Narration.h"
#include "ProjectorObject.h"
#include "GameObject.h"
#include "ButtonTrigger.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "Transform.h"

TLGameEngine::Level8Event::Level8Event()
	:Component(Type::Action)
{

}

TLGameEngine::Level8Event::~Level8Event()
{

}

void TLGameEngine::Level8Event::Awake()
{
	for (auto fileid : m_projectorIDs)
	{
		m_projectors.push_back(SceneManager::Instance().FindObject(fileid)->GetComponent<ProjectorObject>());
	}


	SceneManager::Instance().FindObject(m_interactiveID);
	m_interactive = SceneManager::Instance().FindObject(m_interactiveID);
	m_narration1 = GetComponent<Narration>();
	m_narration2 = SceneManager::Instance().FindObject(m_narrationID)->GetComponent<Narration>();
	m_button = SceneManager::Instance().FindObject(m_buttonID)->GetComponent<ButtonTrigger>();

#ifdef _DEBUG
	assert(m_interactive.lock() != nullptr || m_narration2.lock() != nullptr);
#endif // _DEBUG
}

void TLGameEngine::Level8Event::Start()
{
	m_button.lock()->SetForceTriggerEvent(true);
	m_interactive.lock()->GetComponent<BoxCollider>()->SetIsActive(false);
	m_interactive.lock()->GetComponent<Rigidbody>()->SetIsActive(false);
	m_interactive.lock()->GetComponent<Transform>()->GetChild(0)->GetComponent<MeshRenderer>()->SetIsActive(false);
	m_interactive.lock()->GetComponent<Transform>()->GetChild(1)->GetComponent<MeshRenderer>()->SetIsActive(false);
}

void TLGameEngine::Level8Event::Update()
{
	if (m_bEndEvent)
	{
		return;
	}

	bool m_all2D = true;
	for (auto& projector : m_projectors)
	{
		if (!projector.lock()->GetProjectorOn())
		{
			m_all2D = false;
			break;
		}
	}

	if (closeDoor == false && m_projectors[1].lock()->GetProjectorOn())
	{
		closeDoor = true;
		m_button.lock()->SetForceTriggerEvent(false);
	}

	if (m_projectors[0].lock()->GetProjectorOn() && m_event == 0)
	{
		m_event++;
		m_narration1.lock()->SetPlay(true);
	}

	if (m_all2D && m_event == 1)
	{
		if (!m_narration2.lock()->GetIsPlaying())
		{
			m_event++;
			m_narration2.lock()->SetPlay(true);
		}
	}

	if (m_event == 2 && m_narration2.lock()->GetLine() == 2)
	{
		// m_interactive Object On
		m_bEndEvent = true;


		m_interactive.lock()->GetComponent<BoxCollider>()->SetIsActive(true);
		m_interactive.lock()->GetComponent<Rigidbody>()->SetIsActive(true);
		m_interactive.lock()->GetComponent<Transform>()->GetChild(0)->GetComponent<MeshRenderer>()->SetIsActive(true);
		m_interactive.lock()->GetComponent<Transform>()->GetChild(1)->GetComponent<MeshRenderer>()->SetIsActive(true);

	}
}
