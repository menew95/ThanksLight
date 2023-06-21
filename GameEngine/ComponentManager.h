#pragma once

#include "Singleton.h"
#include "Component.h"

namespace TLGameEngine
{
class Component;


class ComponentManager : public Singleton<ComponentManager>
{
public:
	void Release();

	void Start();
	void Update();
	void Destroy();	//한 프레임의 마지막에 지울 컴포넌트 한번에 정리

	void AddComponent(std::shared_ptr<Component> component);
	void AddComponent(std::vector<std::shared_ptr<Component>>& components, bool isActive = true);
	void RemoveComponent(std::shared_ptr<Component> component);
	
	void SetActive(bool value, std::shared_ptr<Component> component);
	void SetActive(bool value, std::vector<std::shared_ptr<Component>>& components);
private:
	std::array<std::vector<std::shared_ptr<Component>>, static_cast<char>(Component::Type::End)> m_componentsList;

	std::vector<std::shared_ptr<Component>> m_addedComponents;		//새로 추가되는 com을 한 번에 모아서 처리하기 위한 배열
	std::vector<std::shared_ptr<Component>> m_componentsToBeDestroy;	//프레임의 마지막에 한번에 삭제하기 위해 모아두는 배열
	std::vector<std::shared_ptr<Component>> m_inactiveComponents;	//비활성 components
	std::vector<std::shared_ptr<Component>> m_awakeComponents;

	void RemoveAt(std::vector<std::shared_ptr<Component>>& components, std::shared_ptr<Component> component);
};
}