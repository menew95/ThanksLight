#include "pch.h"
#include "Level2Event.h"
#include "SceneManager.h"
#include "Light.h"
#include "Narration.h"
#include "MeshRenderer.h"
#include "SphereCollider.h"
#include "Transform.h"
#include "InteractiveObject.h"
#include "Rigidbody.h"
#include "GameManager.h"


TLGameEngine::Level2Event::Level2Event()
	:Component(Type::Action)
{

}

TLGameEngine::Level2Event::~Level2Event()
{

}

void TLGameEngine::Level2Event::Awake()
{
	m_light = SceneManager::Instance().FindObject(m_lightID)->GetComponent<Light>();
	m_ballGameObject = SceneManager::Instance().FindObject(m_ballID)->GetComponent<Rigidbody>();
	m_narration = SceneManager::Instance().FindObject(m_narrationID)->GetComponent<Narration>();
	m_interativeObject = SceneManager::Instance().FindObject(m_interativeObjectID)->GetComponent<InteractiveObject>();

	m_light.lock()->SetIsActive(false);
	m_ballGameObject.lock()->GetComponent<Rigidbody>()->SetIsActive(false);
	m_ballGameObject.lock()->GetComponent<SphereCollider>()->SetColliderLayer("3DOBJECT");
	m_ballGameObject.lock()->GetComponent<MeshRenderer>()->SetIsActive(false);
	m_ballGameObject.lock()->GetComponent<SphereCollider>()->SetIsActive(false);
}

void TLGameEngine::Level2Event::Update()
{
	if (m_bLightOn)
	{
		m_fTimer += Time::Instance().GetDeltaTime();

		if (m_fTimer > 3.0f)
		{
			OnOffLight(false);
		}
	}
	if (m_narration.lock() != nullptr && m_narration.lock()->GetIsPlaying())
	{
		auto _currNarration = m_narration.lock()->GetLine();

		if (_currNarration == 1)
		{
			GameManager::Instance().SetKeybordInput(false);
		}

		if (_currNarration == 7 && m_event == 0)
		{
			ShootBall();
			m_event++;
		}
		else if(_currNarration == 8 && m_event == 1)
		{
			OnOffLight(true);
			m_event++;
		}
		//else 
	}

	if (!m_narration.lock()->GetIsPlaying() && m_event == m_maxEvent)
	{
		m_endEvent = true;
		m_event++;
	}

	if (m_endEvent)
	{
		GameManager::Instance().SetKeybordInput(true);
	}
}

void TLGameEngine::Level2Event::ShootBall()
{
	m_ballGameObject.lock()->GetComponent<Rigidbody>()->SetIsActive(true);
	m_ballGameObject.lock()->GetComponent<MeshRenderer>()->SetIsActive(true);
	m_ballGameObject.lock()->GetComponent<SphereCollider>()->SetIsActive(true);

	auto dir =
		m_interativeObject.lock()->GetComponent<Transform>()->GetWorldPosition() -
		m_ballGameObject.lock()->GetComponent<Transform>()->GetWorldPosition();

	dir *= 5;

	m_ballGameObject.lock()->AddImpulse({ dir.x, dir.y, dir.z });
}

void TLGameEngine::Level2Event::OnOffLight(bool value)
{
	m_bLightOn = value;
	m_light.lock()->SetIsActive(value);

	if (value == false)
	{
		m_interativeObject.lock()->Set3DObject();
	}
}
