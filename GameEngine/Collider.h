#pragma once
#include "Component.h"
#include "..\Physics\Export\PhysicsInfo.h"

namespace TLGameEngine
{
	class Transform;
	class Rigidbody;

	class Collider :
		public Component
	{
	public:
		Collider();
		Collider(DirectX::SimpleMath::Vector3 center);
		virtual ~Collider();

	public:
		// --------- Component Implement ------------
		virtual void Awake() override;
		virtual void OnEnable() override;
		virtual void OnDisalbe() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void FixedUpdate() override;
		virtual void OnDestroy() override;

	public:
		PhysicsEngine::eGeometryType GetColliderType() const { return m_colliderType; }
		PhysicsEngine::ePhysicsType GetPhysicsType() const { return m_physicsType; }
		void SetPhysicsType(PhysicsEngine::ePhysicsType val) { m_physicsType = val; }
		std::weak_ptr<Transform> GetTransform() { return m_Transform; }

	public:
		void AddTriggerEvent(PhysicsEngine::IEventTrigger* callBackClass);
		void SetColliderLayer(const std::string& val);

		Vector3 GetCenter() const { return m_Center; }
		void SetCenter(Vector3 val) { m_Center = val; }

		bool GetTrigger() const { return m_IsTrigger; }
		void SetTrigger(bool val) { m_IsTrigger = val; }

	protected:
		std::weak_ptr<Transform> m_Transform;
		std::weak_ptr<Rigidbody> m_RigidBody;

		DirectX::SimpleMath::Vector3 m_Center;

		PhysicsEngine::eGeometryType m_colliderType;
		PhysicsEngine::ePhysicsType m_physicsType;
		
		std::string m_ColliderLayer;

		int m_IsTrigger;
	};
}