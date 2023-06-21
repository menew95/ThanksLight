#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

TLGameEngine::Component::Component(Type type)
	: m_type(type)
{
}

TLGameEngine::Component::~Component()
{

}

void TLGameEngine::Component::SetIsActive(bool value)
{
	m_isActive = value;

	if (m_isActive)
	{
		OnEnable();
	}
	else
	{
		OnDisalbe();
	}
}

std::shared_ptr<TLGameEngine::Transform> TLGameEngine::Component::GetTransform()
{
	return m_gameObject.lock()->GetComponent<Transform>();
}
