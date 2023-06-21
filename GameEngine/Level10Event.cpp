#include "pch.h"
#include "Level10Event.h"
#include "SceneManager.h"
#include "Narration.h"
#include "GameManager.h"
#include "Transform.h"
#include "InteractiveObject.h"

TLGameEngine::Level10Event::Level10Event()
	:Component(Type::Action)
{

}

TLGameEngine::Level10Event::~Level10Event()
{

}

void TLGameEngine::Level10Event::Awake()
{
	m_narration = GetComponent<Narration>();

	m_camera = SceneManager::Instance().FindObject(m_cameraID)->GetComponent<Transform>();
	m_object = SceneManager::Instance().FindObject(m_objectID)->GetComponent<Transform>();
	m_interactive1 = SceneManager::Instance().FindObject(m_interactive1ID)->GetComponent<InteractiveObject>();
	m_interactive2 = SceneManager::Instance().FindObject(m_interactive2ID)->GetComponent<InteractiveObject>();
	m_interactive3 = SceneManager::Instance().FindObject(m_interactive3ID)->GetComponent<InteractiveObject>();

#ifdef _DEBUG
	assert(m_narration.lock() != nullptr);
#endif // _DEBUG
}

void TLGameEngine::Level10Event::Update()
{
	if (m_bCheck2D == false)
	{
		if (!m_interactive1.lock()->GetIs2DObject() || !m_interactive2.lock()->GetIs2DObject()
			|| !m_interactive3.lock()->GetIs2DObject())
		{
			// 한개라도 변화시 전부다 변화
			m_bCheck2D = true;
			m_interactive1.lock()->Set3DObject();
			m_interactive2.lock()->Set3DObject();
			m_interactive3.lock()->Set3DObject();
		}


		//m_object.lock()->LookAt(m_camera.lock()->GetWorldPosition());
	}

	if (m_bEndEvnet)
	{
		return;
	}

	if (m_event == 0 && m_narration.lock()->GetIsPlaying())
	{
		m_event++;
		GameManager::Instance().SetKeybordInput(false);
	}

	if (m_event == 1 && !m_narration.lock()->GetIsPlaying())
	{
		m_bEndEvnet = true;
		GameManager::Instance().SetKeybordInput(true);
	}
}
