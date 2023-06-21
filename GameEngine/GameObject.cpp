#include "pch.h"
#include "GameObject.h"
#include "ComponentManager.h"
#include "Transform.h"

void TLGameEngine::GameObject::AddComponent(std::shared_ptr<Component> component)
{
	ComponentManager::Instance().AddComponent(component);
	m_components.push_back(component);
	component->m_gameObject = shared_from_this();
}

void TLGameEngine::GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
	ComponentManager::Instance().RemoveComponent(component);
}

void TLGameEngine::GameObject::SetActive(bool value)
{
	m_IsActive = value;
	// 컴포넌트가 이미 꺼져있건 오브젝트 액티브했을때 꺼져있어야함 지금 구조로는 불가능

	/*for (auto component : m_components)
	{
		if (component->GetIsActiveSelf())
		{
			component->SetIsActive(value);
		}
	}

	auto childs = GetComponent<Transform>()->GetChildren();

	for (auto child : childs)
	{
		child->GetGameObject()->SetActive(false);
	}*/
}
