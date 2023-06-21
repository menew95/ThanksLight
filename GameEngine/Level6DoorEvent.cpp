#include "pch.h"
#include "Level6DoorEvent.h"
#include "Transform.h"
#include "InteractiveObject.h"
#include "SceneManager.h"

TLGameEngine::Level6DoorEvent::Level6DoorEvent() :
	Component(Component::Type::Action)
{

}

TLGameEngine::Level6DoorEvent::~Level6DoorEvent()
{
}

void TLGameEngine::Level6DoorEvent::Awake()
{
	m_Object = GetComponent<InteractiveObject>();
}

void TLGameEngine::Level6DoorEvent::Update()
{
	if (!m_Object.lock()->GetIs2DObject())
	{
		Vector3 trashcan = { 0.f, 20.f, 100.f };

		GetTransform()->SetWorldPosition(trashcan);
	}
}
