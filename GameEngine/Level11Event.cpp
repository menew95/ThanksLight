#include "pch.h"
#include "Level11Event.h"
#include "SceneManager.h"
#include "Narration.h"
#include "GameManager.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "InteractiveObject.h"
#include "MyTime.h"
#include "TableSnapTrigger.h"
#include "PhysicsManager.h"

TLGameEngine::Level11Event::Level11Event()
	:Component(Type::Action)
{

}

TLGameEngine::Level11Event::~Level11Event()
{

}

void TLGameEngine::Level11Event::Awake()
{
	m_narration = SceneManager::Instance().FindObject(m_narrationID)->GetComponent<Narration>();
	m_spotLight = SceneManager::Instance().FindObject(m_spotLightID)->GetComponent<Transform>();
	m_interactive = SceneManager::Instance().FindObject(m_interactiveID)->GetComponent<Transform>();

	m_trigger = GetComponent<TableSnapTrigger>();

	for (auto& id : m_bildbordIDs)
	{
		m_bildbords.push_back(SceneManager::Instance().FindObject(id)->GetComponent<InteractiveObject>());
	}
}

void TLGameEngine::Level11Event::Update()
{
	if (m_bEndEvnet)
	{
		return;
	}

	if (m_timerSet)
	{
		m_timer += Time::Instance().GetDeltaTime();
		if (m_timer > 2.0f)
		{
			m_timerSet = false;
			m_trigger.lock()->OnOff(true);
			m_isSetting = false;
		}
	}

	if (!m_isSetting && m_trigger.lock()->GetIsSnap())
	{
		UpdateBildBordSize();
		m_isSetting = true;
	}
	if (m_isSetting && !m_bildbords[m_count].lock()->GetIs2DObject())
	{
		//빌보드가 3D화 되면
		m_count++;

		m_interactive.lock()->GetComponent<Rigidbody>()->AddImpulse({ 0, 7, 3 });
		m_interactive.lock()->GetComponent<Rigidbody>()->SetGravity(true);
		m_timer = 0.0f;
		m_timerSet = true;
		m_trigger.lock()->OnOff(false);
		if (m_count > 10)
		{
			// 너무 많이 만들었다
			m_bEndEvnet = true;
		}
	}
}

void TLGameEngine::Level11Event::UpdateBildBordSize()
{
	DirectX::SimpleMath::Vector3 _pos = { 0,1.2, 5 };
	auto _transform = m_bildbords[m_count].lock()->GetTransform();
	_transform->SetLocalPosition(_pos);

	float _d1 = m_interactive.lock()->GetWorldPosition().z - m_spotLight.lock()->GetWorldPosition().z - 0.25;
	float _d2 = _transform->GetWorldPosition().z - m_spotLight.lock()->GetWorldPosition().z;

	float _scale = _d2 / _d1 * 0.25;
	Vector3 test(_scale, _scale, _scale);
	_transform->SetLocalScale(test);

	m_bildbords[m_count].lock()->GetComponent<BoxCollider>()->SetSize(test * 2);
	
	PhysicsManager::Instance().DeleteObject(m_bildbords[m_count].lock()->GetCollider().get());
	m_bildbords[m_count].lock()->GetCollider()->Awake();

	m_bildbords[m_count].lock()->GetRigidBody()->SetGravity(false);
	
}
