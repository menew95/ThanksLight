#include "pch.h"
#include "ComponentManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "Component.h"
#include "SceneManager.h"
#include "PhysicsManager.h"

void TLGameEngine::ComponentManager::Release()
{
	m_awakeComponents.clear();
	m_addedComponents.clear();
	m_inactiveComponents.clear();
	m_componentsToBeDestroy.clear();
	for (auto& components : m_componentsList)
	{
		components.clear();
	}

	delete& Instance();
}

void TLGameEngine::ComponentManager::Start()
{
	//이전 프레임에 추가된 컴포넌트 한 번에 추가
	for (auto com : m_addedComponents)
	{
		Component::Type type = com->GetType();
		m_componentsList[static_cast<char>(type)].push_back(com);
	}

	for (auto component : m_awakeComponents)
	{
		component->Awake();
	}

	for (auto component : m_addedComponents)
	{
		component->Start();
	}

	m_awakeComponents.clear();
	m_addedComponents.clear();
}

void TLGameEngine::ComponentManager::Update()
{
	const float fixedUpdateTime = 1.f / 60.f;
	static float deltaTime = 0.f;

	deltaTime += TLGameEngine::Time::Instance().GetDeltaTime();

	for (auto& components : m_componentsList)
	{
		for (auto& com : components)
		{
			if (com->GetIsActive()/* && com->GetIsActiveSelf()*/)
			{
				com->FixedUpdate();
			}
		}
	}

	TLGameEngine::PhysicsManager::Instance().Simulate(TLGameEngine::Time::Instance().GetDeltaTime());

	for (auto& components : m_componentsList)
	{
		for (auto& com : components)
		{
			if (com->GetIsActive()/* && com->GetIsActiveSelf()*/)
			{
				com->Update();
			}
		}
	}

	for (auto& components : m_componentsList)
	{
		for (auto& com : components)
		{
			if (com->GetIsActive()/* && com->GetIsActiveSelf()*/)
			{
				com->LateUpdate();
			}
		}
	}

	for (auto& components : m_componentsList)
	{
		for (auto& com : components)
		{
			if (com->GetIsActive()/* && com->GetIsActiveSelf()*/)
			{
				com->OnDrawGizmo();
			}
		}
	}
}

void TLGameEngine::ComponentManager::Destroy()
{
	for (auto& destroyComponent : m_componentsToBeDestroy)
	{
		if (destroyComponent->GetIsActive())
		{
			auto& components = m_componentsList[static_cast<char>(destroyComponent->GetType())];
			RemoveAt(components, destroyComponent);
		}
		else
		{
			RemoveAt(m_inactiveComponents, destroyComponent);
		}
	}

	m_componentsToBeDestroy.clear();
}

void TLGameEngine::ComponentManager::AddComponent(std::shared_ptr<Component> component)
{
	m_addedComponents.push_back(component);
	m_awakeComponents.push_back(component);
}

void TLGameEngine::ComponentManager::AddComponent(std::vector<std::shared_ptr<Component>>& components, bool isActive)
{
	assert(false && "컴포넌트 엑티브 설정 구현해야함");
	if (isActive)
	{
		m_addedComponents.insert(m_addedComponents.end(), components.begin(), components.end());
	}
	else
	{
		m_inactiveComponents.insert(m_inactiveComponents.end(), components.begin(), components.end());
	}

	m_awakeComponents.insert(m_awakeComponents.end(), components.begin(), components.end());
}

void TLGameEngine::ComponentManager::RemoveComponent(std::shared_ptr<Component> component)
{
	m_componentsToBeDestroy.push_back(component);
}

void TLGameEngine::ComponentManager::SetActive(bool value, std::shared_ptr<Component> component)
{
	if (value)
	{
		RemoveAt(m_inactiveComponents, component);
		m_componentsList[static_cast<char>(component->GetType())].push_back(component);
	}
	else
	{
		auto components = m_componentsList[static_cast<char>(component->GetType())];
		RemoveAt(components, component);
		m_inactiveComponents.push_back(component);
	}
	component->SetIsActive(value);
}

void TLGameEngine::ComponentManager::SetActive(bool value, std::vector<std::shared_ptr<Component>>& components)
{
	for (auto com : components)
	{
		SetActive(value, com);
	}
}

void TLGameEngine::ComponentManager::RemoveAt(std::vector<std::shared_ptr<Component>>& components, std::shared_ptr<Component> component)
{
	for (auto iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter) == component)
		{
			*iter = *(components.end() - 1);
			components.resize(components.size() - 1);
			break;
		}
	}
}
