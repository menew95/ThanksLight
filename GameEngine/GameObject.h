#pragma once

#include "Object.h"
#include "LayerMask.h"

namespace TLGameEngine
{
	class Component;
	class Transform;

	class GameObject : public Common::Object, public std::enable_shared_from_this<GameObject>
	{
	public:
		const std::string& GetName() { return m_Name; }
		void SetName(std::string val) { m_Name = val; }
	
		std::string GetTag() const { return m_TagString; }
		void SetTag(std::string val) { m_TagString = val; }

		template<typename T>
		std::shared_ptr<T> GetComponent();
		template<typename T>
		std::shared_ptr<T> AddComponent();

		void AddComponent(std::shared_ptr<Component> component);

		void RemoveComponent(std::shared_ptr<Component> component);
		std::vector<std::shared_ptr<Component>>& GetComponents() { return m_components; }

		LayerMask& GetLayer() { return m_layer; }

		void SetActive(bool value);
		bool GetActive() { return m_ActiveSelf; }

	private:
		std::string m_Name = "";
		std::vector<std::shared_ptr<Component>> m_components = {};

		LayerMask m_layer;
		std::string m_TagString;
		int m_IsActive = true;

		bool m_ActiveSelf = true;	// true => Update false => Not Update
		bool m_IsStatic;	// if enabled true and call onEnabled

		bool m_isAwakeCall;	// true => Update false => Not Update

		std::vector<FileID> m_Component;
		BOOST_DESCRIBE_CLASS(GameObject, (), (), (), (m_Name, m_TagString, m_IsActive))
	};
	template<typename T>
	inline std::shared_ptr<T> GameObject::GetComponent()
	{
		for (auto& com : m_components)
		{
			if (typeid(*com).name() == typeid(T).name())
			{
				return std::static_pointer_cast<T>(com);
			}
		}
		return nullptr;
	}

	template<typename T>
	inline std::shared_ptr<T>
		GameObject::AddComponent()
	{
		std::shared_ptr<T> _component = std::make_shared<T>();

		this->AddComponent(_component);
	}

}

