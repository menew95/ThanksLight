#pragma once

/// <summary>
/// 물리엔진 신에 존재하는 오브젝트
/// </summary>
/// 
namespace PhysicsEngine
{
	class Shape;

	class RigidActor
	{

	public:
		RigidActor(physx::PxPhysics* physics, ePhysicsType type, physx::PxTransform transform);
		RigidActor(physx::PxRigidActor* Actor);

		~RigidActor();

		bool AttachShape(std::wstring shapeName, physx::PxShape* shape, float density);
		bool DetachShape(std::wstring shapeName);
		bool DetachAllShape();

		// 물리관련
		bool ClearAllForce();
		bool AddForce(physx::PxVec3 force);
		bool AddImpulse(physx::PxVec3 force);
		bool AddTorque(physx::PxVec3 force);
		bool AddTorqueImpulse(physx::PxVec3 force);
		bool SetLinearDamping(float damping);
		bool SetAngularDamping(float damping);
		bool SetLinearVelocity(physx::PxVec3 vel);
		bool SetAnguarVelocity(physx::PxVec3 vel);
		bool SetTransform(physx::PxTransform transform);
		bool SetGravity(bool flag);
		bool SetMass(float mass);

		//Get, Set
	public:

		physx::PxRigidActor* GetPhysicsActor() const { return m_physicsActor; }

	private:
		RigidActor() = default;

	private:
		void UpdateDensity();

	protected:
		ePhysicsType m_rigidActorType;

		std::unordered_map<std::wstring, std::shared_ptr<Shape>> m_physicsShapes;
		physx::PxRigidActor* m_physicsActor;

	};
}
