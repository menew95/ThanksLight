#pragma once

#include <queue>
#include <Singleton.h>

#include "..\Physics\Export\IPhysicsEngine.h"

namespace TLGameEngine
{
	class GameObject;
	class Collider;

	struct RayCastHit
	{
		RayCastHit()
			:m_objName("")
			, m_disance(0.0f)
			, m_isHit(false)
			, m_gameObject(nullptr)
		{

		}
		RayCastHit(const char* name, float dis, bool hit, std::shared_ptr<GameObject> gameObject)
			:m_objName(name)
			, m_disance(dis)
			, m_isHit(hit)
			, m_gameObject(gameObject)
		{ }
		bool m_isHit;
		std::string m_objName;
		std::shared_ptr<GameObject> m_gameObject;
		float m_disance;
	};

	class PhysicsManager :
		public Singleton<PhysicsManager>
	{
	public:
		PhysicsManager();
		virtual ~PhysicsManager();

	public:
		//Object 생성및 상태관리
		void AddObject(Collider* collider);
		void ArrangeShape(Collider* collider);
		void DeleteObject(Collider* collider);

		void DisableObject(Collider* collider);
		void EnableObject(Collider* collider);

		//오브젝트 
		void AddForce(Collider* collider, PhysicsEngine::Vector3 force);
		void AddImpulse(Collider* collider, PhysicsEngine::Vector3 impulse);
		void SetTransform(Collider* collider, PhysicsEngine::Transform worldtransform);

		void SetMass(Collider* collider, float mass);
		void SetLinearDamp(Collider* collider, float damping);
		void SetAngularDamp(Collider* collider, float damping);
		void SetAxisLock(Collider* collider, PhysicsEngine::PhysicsAixsLock lockInfo);

		void SetMaxLinearVelocity(Collider* collider, float vel);
		void SetMaxAngularVelocity(Collider* collider, float vel);

		void SetLinearVelocity(Collider* collider, PhysicsEngine::Vector3 velocity);
		void SetAngularVelocity(Collider* collider, PhysicsEngine::Vector3 velocity);

		Vector3 GetAngularVelocity(Collider* collider);
		Vector3 GetLinearVelocity(Collider* collider);

		void SetGravity(Collider* collider, bool flag);
		void SetKinematic(Collider* collider, bool flag);

		void AddTriggerEvent(Collider* collider, PhysicsEngine::IEventTrigger*);
		void AddColliderEvent(Collider* collider, PhysicsEngine::IEventCollider*);

		void Simulate(float dt);

		void UpdateTransform(Collider* collider);
		void SetColliderLayer(Collider* collider, std::string LayerName);

		//TLGameEngine::RayCastHit RayCast(PhysicsEngine::Vector3 origin, PhysicsEngine::Vector3 dir, float distance);
		TLGameEngine::RayCastHit RayCast(PhysicsEngine::Vector3 origin, PhysicsEngine::Vector3 dir, float distance, std::vector<std::string> Filteredlayers, PhysicsEngine::ePhysicsType includeType );


		void InitPhysics();
	private:
		std::shared_ptr<PhysicsEngine::IPhysicsEngine> m_phisicsEngine;
    };
}

