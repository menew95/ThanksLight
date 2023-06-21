#pragma once

#include "Object.h"
#include "GameObject.h"
namespace TLGameEngine
{
class Transform;
class GameObject;

class Component : public Common::Object
{
public:
	enum class Type
	{
		Physics,
		Action,
		Camera,
		Rendering,
		End
	};
public:
	Component(Type type);
	virtual ~Component();

public:
	virtual void Awake() {};
	virtual void OnEnable() {}
	virtual void OnDisalbe() {}
	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void FixedUpdate() {};
	virtual void OnDestroy() {};
	virtual void OnDrawGizmo() {};

	bool GetIsActive() { return m_isActive; }
	void SetIsActive(bool value);
	bool GetIsActiveSelf() { return m_isActiveSelf; }
	void SetIsActiveSelf(bool value) { m_isActiveSelf = value; }
	std::shared_ptr<Transform> GetTransform();
	std::shared_ptr<GameObject> GetGameObject() { return m_gameObject.lock();  }

	Type GetType() const { return m_type; }

public:
	template<typename T>
	inline std::shared_ptr<T> GetComponent();
	template<typename T>
	inline std::shared_ptr<T> AddComponent();

protected:
	std::weak_ptr<GameObject> m_gameObject;
	Type m_type;

	bool m_isActive = true;
	bool m_isActiveSelf = true;

	friend GameObject;
};

template<typename T>
inline std::shared_ptr<T> TLGameEngine::Component::GetComponent()
{
	//assert(false);
	if (m_gameObject.lock() != nullptr)
	{
		return m_gameObject.lock()->template GetComponent<T>();
	}

	return nullptr;
}

template<typename T>
inline std::shared_ptr<T> TLGameEngine::Component::AddComponent()
{
	if (m_gameObject.lock() != nullptr)
	{
		return m_gameObject.lock()->template AddComponent<T>();
	}

	return nullptr;
}
}