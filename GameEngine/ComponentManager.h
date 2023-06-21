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
	void Destroy();	//�� �������� �������� ���� ������Ʈ �ѹ��� ����

	void AddComponent(std::shared_ptr<Component> component);
	void AddComponent(std::vector<std::shared_ptr<Component>>& components, bool isActive = true);
	void RemoveComponent(std::shared_ptr<Component> component);
	
	void SetActive(bool value, std::shared_ptr<Component> component);
	void SetActive(bool value, std::vector<std::shared_ptr<Component>>& components);
private:
	std::array<std::vector<std::shared_ptr<Component>>, static_cast<char>(Component::Type::End)> m_componentsList;

	std::vector<std::shared_ptr<Component>> m_addedComponents;		//���� �߰��Ǵ� com�� �� ���� ��Ƽ� ó���ϱ� ���� �迭
	std::vector<std::shared_ptr<Component>> m_componentsToBeDestroy;	//�������� �������� �ѹ��� �����ϱ� ���� ��Ƶδ� �迭
	std::vector<std::shared_ptr<Component>> m_inactiveComponents;	//��Ȱ�� components
	std::vector<std::shared_ptr<Component>> m_awakeComponents;

	void RemoveAt(std::vector<std::shared_ptr<Component>>& components, std::shared_ptr<Component> component);
};
}