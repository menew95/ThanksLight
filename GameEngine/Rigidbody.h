#pragma once
#include "Component.h"
#include "..\Physics\Export\PhysicsInfo.h"

namespace TLGameEngine
{
	class Collider;

	class Rigidbody :
	    public Component
	{
	public:
		Rigidbody();
		virtual ~Rigidbody();

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
		// --------- RigidBody Functions ------------
		//오브젝트 
		virtual void AddForce(PhysicsEngine::Vector3 force);
		virtual void AddImpulse(PhysicsEngine::Vector3 impulse);

		virtual void SetMass(float mass);
		virtual void SetLinearDamp(float damping);
		virtual void SetAngularDamp(float damping);

		virtual void SetMaxLinearVelocity(float vel);
		virtual void SetMaxAngularVelocity(float vel);

		virtual void SetLinearVelocity(PhysicsEngine::Vector3 velocity);
		virtual void SetAngularVelocity(PhysicsEngine::Vector3 velocity);

		virtual Vector3 GetAngularVelocity();
		virtual Vector3 GetLinearVelocity();

		void SetPhysicsAxisLock(bool linearX, bool linearY, bool linearZ, bool angularX, bool angularY, bool angularZ);
		virtual void SetGravity(bool flag);
		virtual void SetKinematic(bool flag);

		void UpdateRigidBodySetting();

	private:
		virtual void SetAxisLock(PhysicsEngine::PhysicsAixsLock lockInfo);

	private:
		std::weak_ptr<Collider> m_collider;
		////rigidbody 이사예정
		PhysicsEngine::PhysicsAixsLock m_physicsAxisLock;

		float m_Mass;

		float m_damp;
		float m_angularDamp;
		int m_UseGravity;
		int m_IsKinematic;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Rigidbody, (), (), (), (m_Mass, m_UseGravity, m_IsKinematic))
	};
}