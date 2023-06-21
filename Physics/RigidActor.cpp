#include "pch.h"
#include "RigidActor.h"
#include "Shape.h"

PhysicsEngine::RigidActor::RigidActor(physx::PxPhysics* physics, ePhysicsType type, physx::PxTransform transform)
{
	m_rigidActorType = type;

	switch (m_rigidActorType)
	{
	case ePhysicsType::eSTATTIC:
		{
			m_physicsActor = physics->createRigidStatic(transform);
			break;
		}
	case ePhysicsType::eDYNAMIC:
		{
			m_physicsActor = physics->createRigidDynamic(transform);
			break;
		}
	default:
		{
			m_physicsActor = nullptr;
			PX_ASSERT(false);
			break;
		}
	}
}

PhysicsEngine::RigidActor::RigidActor(physx::PxRigidActor* Actor)
{
	m_physicsActor = Actor;
}

void PhysicsEngine::RigidActor::UpdateDensity()
{
	if(m_rigidActorType == ePhysicsType::eSTATTIC)
	{
		return;
	}

	UINT numOfShapes = m_physicsShapes.size();
	if (numOfShapes == 0)
	{
		return;
	}

	std::vector<physx::PxReal> densities(numOfShapes);
	physx::PxRigidDynamic* actor = dynamic_cast<physx::PxRigidDynamic*>(m_physicsActor); 
	if (actor == nullptr)
	{
		return;
	}

	//TODO:: 쉐이프 별 밀도 개별적용코드를 짤 것. 지금은 일괄적으로 통일해버림.
	//for (int shapeIdx = 0; shapeIdx < numOfShapes; shapeIdx++)
	//{
	//}
	physx::PxRigidBodyExt::updateMassAndInertia(*actor, 1.0f);

	//physx::PxRigidBodyExt::updateMassAndInertia(*m_physicsActor, );
}

PhysicsEngine::RigidActor::~RigidActor()
{
	m_physicsActor->release();
	m_physicsActor = nullptr;

	for (auto shapes : m_physicsShapes)
	{
		shapes.second.reset();
		shapes.second = nullptr;
	}

	m_physicsShapes.clear();
}

bool PhysicsEngine::RigidActor::AttachShape(std::wstring shapeName, physx::PxShape* shape, float density)
{
	if (shape == nullptr)
	{
		return false;
	}

	std::shared_ptr<Shape> tempShape = std::make_shared<Shape>(shape, density);

	m_physicsShapes.emplace(shapeName, tempShape);
	m_physicsActor->attachShape(*shape);

	UpdateDensity();

	return true;
}

bool PhysicsEngine::RigidActor::DetachShape(std::wstring shapeName)
{
	auto foundedShape = m_physicsShapes.find(shapeName);
	if (foundedShape == m_physicsShapes.end())
	{
		return false;
	}

	m_physicsActor->detachShape(*foundedShape->second->GetShape());
	m_physicsShapes.erase(shapeName);
	return true;
}

bool PhysicsEngine::RigidActor::DetachAllShape()
{
	for (auto shape : m_physicsShapes)
	{
		m_physicsActor->detachShape(*shape.second->GetShape());
	}
	m_physicsShapes.clear();

	return true;
}

bool PhysicsEngine::RigidActor::ClearAllForce()
{
	bool result1 = SetLinearVelocity({0.f, 0.f, 0.f});
	bool result2 = SetAnguarVelocity({0.f, 0.f, 0.f});

	return (result1 && result2);
}

bool PhysicsEngine::RigidActor::AddForce(physx::PxVec3 force)
{
	if (m_rigidActorType == ePhysicsType::eDYNAMIC)
	{
		((physx::PxRigidDynamic*)m_physicsActor)->addForce(force, physx::PxForceMode::eFORCE, true);
		return true;
	}

	return false;
}

bool PhysicsEngine::RigidActor::AddImpulse(physx::PxVec3 force)
{
	if (m_rigidActorType == ePhysicsType::eDYNAMIC)
	{
		((physx::PxRigidDynamic*)m_physicsActor)->addForce(force, physx::PxForceMode::eIMPULSE, true);
		return true;
	}

	return false;
}

bool PhysicsEngine::RigidActor::AddTorque(physx::PxVec3 force)
{
	if (m_rigidActorType == ePhysicsType::eDYNAMIC)
	{
		((physx::PxRigidDynamic*)m_physicsActor)->addTorque(force, physx::PxForceMode::eFORCE, true);
		return true;
	}

	return false;
}

bool PhysicsEngine::RigidActor::AddTorqueImpulse(physx::PxVec3 force)
{
	if (m_rigidActorType == ePhysicsType::eDYNAMIC)
	{
		((physx::PxRigidDynamic*)m_physicsActor)->addTorque(force, physx::PxForceMode::eIMPULSE, true);
		return true;
	}

	return false;
}

bool PhysicsEngine::RigidActor::SetLinearDamping(float damping)
{
	if (m_rigidActorType == ePhysicsType::eDYNAMIC)
	{
		((physx::PxRigidDynamic*)m_physicsActor)->setLinearDamping(damping);
		return true;
	}

	return false;
}

bool PhysicsEngine::RigidActor::SetAngularDamping(float damping)
{
	if (m_rigidActorType == ePhysicsType::eDYNAMIC)
	{
		((physx::PxRigidDynamic*)m_physicsActor)->setAngularDamping(damping);
		return true;
	}

	return false;
}

bool PhysicsEngine::RigidActor::SetLinearVelocity(physx::PxVec3 vel)
{
	if (m_rigidActorType == ePhysicsType::eDYNAMIC)
	{
		((physx::PxRigidDynamic*)m_physicsActor)->setLinearVelocity(vel);
		return true;
	}

	return false;
}

bool PhysicsEngine::RigidActor::SetAnguarVelocity(physx::PxVec3 vel)
{
	if (m_rigidActorType == ePhysicsType::eDYNAMIC)
	{
		((physx::PxRigidDynamic*)m_physicsActor)->setAngularVelocity(vel);
		return true;
	}

	return false;
}

bool PhysicsEngine::RigidActor::SetTransform(physx::PxTransform transform)
{
	m_physicsActor->setGlobalPose(transform);
	return true;
}

bool PhysicsEngine::RigidActor::SetGravity(bool flag)
{
	if (flag)
	{
		m_physicsActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
	}
	else
	{
		m_physicsActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	}

	return true;
}

bool PhysicsEngine::RigidActor::SetMass(float mass)
{
	bool result;

	if (m_rigidActorType == ePhysicsType::eDYNAMIC)
	{
		result =  physx::PxRigidBodyExt::setMassAndUpdateInertia(*(physx::PxRigidDynamic*)m_physicsActor, mass);
	}
	else
	{
		result = false;
	}

	return result;
}
