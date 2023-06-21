#include "pch.h"
#include "Door.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "AudioSource.h"

TLGameEngine::Door::Door():
	Component(Component::Type::Action)
{
	m_DoorOpenLerp = 0.0f;
	m_isDoorOpen = false;
}

TLGameEngine::Door::~Door()
{

}

void TLGameEngine::Door::Awake()
{
	m_leftDoor = SceneManager::Instance().FindObject(m_leftDoorID)->GetComponent<Transform>();
	m_rightDoor = SceneManager::Instance().FindObject(m_rightDoorID)->GetComponent<Transform>();

	m_frontLight = SceneManager::Instance().FindObject(m_frontLightID)->GetComponent<MeshRenderer>();
	m_backLight = SceneManager::Instance().FindObject(m_backLightID)->GetComponent<MeshRenderer>();

	m_openMaterial = ResourceManager::Instance().GetMaterial(m_openMaterialID.guid);
	m_closeMaterial = ResourceManager::Instance().GetMaterial(m_closeMaterialID.guid);

	m_LeftDoorBasePosition = m_leftDoor.lock()->GetWorldPosition();
	m_RightDoorBasePosition = m_rightDoor.lock()->GetWorldPosition();

	if (m_RightDoorDirection.Length() == 0)
	{
		m_RightDoorDirection = { 0, 0, 1 };
	}


	// 문에 사운드 컴포넌트를 달았다.
	m_audioSource = m_gameObject.lock()->GetComponent<AudioSource>();
	assert(m_audioSource != nullptr);
}

void TLGameEngine::Door::OnEnable()
{

}

void TLGameEngine::Door::OnDisalbe()
{

}

void TLGameEngine::Door::Start()
{

}

void TLGameEngine::Door::Update()
{
	Vector3 moveDelta;

	if (m_isDoorOpen)
	{
		m_DoorOpenLerp += Time::Instance().GetDeltaTime();
		if (m_DoorOpenLerp > 1.0f)
		{
			m_DoorOpenLerp = 1.0f;
		}
		moveDelta = m_RightDoorDirection * m_DoorOpenLerp;
	}
	else
	{
		m_DoorOpenLerp -= Time::Instance().GetDeltaTime();
		if (m_DoorOpenLerp < 0.0f)
		{
			m_DoorOpenLerp = 0.0f;
		}
		moveDelta = m_RightDoorDirection * m_DoorOpenLerp;
	}
	auto rightDoorPos = m_RightDoorBasePosition + moveDelta;
	auto leftDoorPos = m_LeftDoorBasePosition - moveDelta;

	m_leftDoor.lock()->SetWorldPosition(leftDoorPos);
	m_rightDoor.lock()->SetWorldPosition(rightDoorPos);

}

void TLGameEngine::Door::LateUpdate()
{

}

void TLGameEngine::Door::FixedUpdate()
{

}

void TLGameEngine::Door::OnDestroy()
{

}

void TLGameEngine::Door::SetDoorOpen(bool flag)
{
#ifdef _DEBUG
	if (m_audioSource != nullptr)
#endif // _DEBUG
	{
		if (m_isDoorOpen != flag)
		{
			m_audioSource->SetClip(flag ? m_openClipID.guid : m_closeClipID.guid);
			m_audioSource->stop();
			m_audioSource->play();
		}
	}

	m_isDoorOpen = flag;
}

void TLGameEngine::Door::SetDoorEmissive(bool flag)
{
	if (flag)
	{
		m_frontLight.lock()->SetMaterial(m_openMaterial, 0);
		m_backLight.lock()->SetMaterial(m_openMaterial, 0);
	}
	else
	{
		m_frontLight.lock()->SetMaterial(m_closeMaterial, 0);
		m_backLight.lock()->SetMaterial(m_closeMaterial, 0);
	}
}

void TLGameEngine::Door::SetDoor(std::shared_ptr<GameObject> leftDoor, std::shared_ptr<GameObject> rightDoor)
{
	m_leftDoor = leftDoor->GetComponent<Transform>();
	m_rightDoor = rightDoor->GetComponent<Transform>();

	m_LeftDoorBasePosition = m_leftDoor.lock()->GetWorldPosition();
	m_RightDoorBasePosition = m_rightDoor.lock()->GetWorldPosition();
}
