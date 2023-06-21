#include "pch.h"
#include "PrologueEvent.h"
#include "Narration.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "Narration.h"
#include "AudioSource.h"
#include "GameManager.h"

TLGameEngine::PrologueEvent::PrologueEvent()
	:Component(Type::Action)
{

}

TLGameEngine::PrologueEvent::~PrologueEvent()
{

}

void TLGameEngine::PrologueEvent::Awake()
{
	m_light1 = SceneManager::Instance().FindObject(m_light1ID)->GetComponent<Light>();
	m_light2 = SceneManager::Instance().FindObject(m_light2ID)->GetComponent<Light>();
	m_light3 = SceneManager::Instance().FindObject(m_light3ID)->GetComponent<Light>();
	m_light4 = SceneManager::Instance().FindObject(m_light4ID)->GetComponent<Light>();
	m_light5 = SceneManager::Instance().FindObject(m_light5ID)->GetComponent<Light>();
	m_light6 = SceneManager::Instance().FindObject(m_light6ID)->GetComponent<Light>();
	m_lightEmmisive1 = SceneManager::Instance().FindObject(m_lightEmmisive1ID)->GetComponent<MeshRenderer>();
	m_lightEmmisive2 = SceneManager::Instance().FindObject(m_lightEmmisive2ID)->GetComponent<MeshRenderer>();
	m_lightEmmisive3 = SceneManager::Instance().FindObject(m_lightEmmisive3ID)->GetComponent<MeshRenderer>();
	m_lightEmmisive4 = SceneManager::Instance().FindObject(m_lightEmmisive4ID)->GetComponent<MeshRenderer>();

	m_emmisiveMaterial = ResourceManager::Instance().GetMaterial(m_emmisiveMaterialID.guid);
	m_narration = GetComponent<Narration>();
	m_audioSource = GetComponent<AudioSource>();
#ifdef _DEBUG
	assert(m_light1.lock() != nullptr || m_light2.lock() != nullptr || m_light3.lock() != nullptr || m_light4.lock() != nullptr || m_light5.lock() != nullptr
		|| m_lightEmmisive1.lock() != nullptr || m_lightEmmisive2.lock() != nullptr || m_lightEmmisive3.lock() != nullptr || m_lightEmmisive4.lock() != nullptr);

	assert(m_narration.lock() != nullptr);
	assert(m_audioSource.lock() != nullptr);
#endif // _DEBUG
	m_light1.lock()->SetIsActive(false);
	m_light2.lock()->SetIsActive(false);
	m_light3.lock()->SetIsActive(false);
	m_light4.lock()->SetIsActive(false);
	m_light5.lock()->SetIsActive(false);
	m_light6.lock()->SetIsActive(false);
}

void TLGameEngine::PrologueEvent::Update()
{
	if (m_bLightOn)
	{
		m_fTimer += Time::Instance().GetDeltaTime();

		// 하드 코딩 박아 ! 불 켜지는 속도 조절
		float deltaSpeed = 1;

		if (m_ilightOnCount == 3 && m_fTimer > deltaSpeed * 4)
		{
			m_light4.lock()->SetIsActive(true);
			m_light4.lock()->SetIsActive(true);
			m_light6.lock()->SetIsActive(true);
			m_lightEmmisive4.lock()->SetMaterial(m_emmisiveMaterial, 0);
			m_ilightOnCount++;

			m_audioSource.lock()->stop();
			m_audioSource.lock()->play();

			GameManager::Instance().SetKeybordInput(true);
		}
		else if (m_ilightOnCount == 2 && m_fTimer > deltaSpeed * 3)
		{
			m_light3.lock()->SetIsActive(true);
			m_lightEmmisive3.lock()->SetMaterial(m_emmisiveMaterial, 0);
			m_ilightOnCount++;

			m_audioSource.lock()->stop();
			m_audioSource.lock()->play();
		}
		else if (m_ilightOnCount == 1 && m_fTimer > deltaSpeed * 2)
		{
			m_light2.lock()->SetIsActive(true);
			m_lightEmmisive2.lock()->SetMaterial(m_emmisiveMaterial, 0);
			m_ilightOnCount++;

			m_audioSource.lock()->stop();
			m_audioSource.lock()->play();
		}
		else if (m_ilightOnCount == 0 && m_fTimer > deltaSpeed * 1)
		{
			m_light1.lock()->SetIsActive(true);
			m_lightEmmisive1.lock()->SetMaterial(m_emmisiveMaterial, 0);
			m_ilightOnCount++;

			m_audioSource.lock()->SetClip(m_lightClipID.guid);
			m_audioSource.lock()->play();
		}
	}
	else if (m_bStart)
	{
		if (m_narration.lock()->GetIsPlaying() == false)
		{
			m_bLightOn = true;
		}
	}
	else if (m_bStart == false && m_narration.lock()->GetIsPlaying() == true)
	{
		GameManager::Instance().SetKeybordInput(false);

		m_bStart = true;
		GetComponent<BoxCollider>()->SetIsActive(false);
	}
}
