#include "pch.h"
#include "Rigidbody.h"
#include "PhysicsManager.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"

#include "Collider.h"
#include "GameObject.h"

TLGameEngine::Rigidbody::Rigidbody() :
	Component(Type::Physics),
	m_Mass(1.f),
	m_damp(0.f),
	m_angularDamp(0.05f),
	m_UseGravity(true),
	m_IsKinematic(false)
{

}

TLGameEngine::Rigidbody::~Rigidbody()
{
}

void TLGameEngine::Rigidbody::Awake()
{
	m_collider = m_gameObject.lock()->GetComponent<BoxCollider>();
	if (m_collider.expired())
	{
		m_collider = m_gameObject.lock()->GetComponent<SphereCollider>();
		if (m_collider.expired())
		{
			m_collider = m_gameObject.lock()->GetComponent<CapsuleCollider>();
			if (m_collider.expired())
			{
				m_collider = m_gameObject.lock()->GetComponent<MeshCollider>();
				if (m_collider.expired())
				{
					assert(false);
				}
			}
		}
	}

	UpdateRigidBodySetting();
}

void TLGameEngine::Rigidbody::OnEnable()
{
}

void TLGameEngine::Rigidbody::OnDisalbe()
{
}

void TLGameEngine::Rigidbody::AddForce(PhysicsEngine::Vector3 force)
{
	PhysicsManager::Instance().AddForce(m_collider.lock().get(), force);
}

void TLGameEngine::Rigidbody::AddImpulse(PhysicsEngine::Vector3 impulse)
{
	PhysicsManager::Instance().AddImpulse(m_collider.lock().get(), impulse);
}

void TLGameEngine::Rigidbody::SetMass(float mass)
{
	PhysicsManager::Instance().SetMass(m_collider.lock().get(), mass);
}

void TLGameEngine::Rigidbody::SetLinearDamp(float damping)
{
	PhysicsManager::Instance().SetLinearDamp(m_collider.lock().get(), damping);
}

void TLGameEngine::Rigidbody::SetAngularDamp(float damping)
{
	PhysicsManager::Instance().SetAngularDamp(m_collider.lock().get(), damping);
}

void TLGameEngine::Rigidbody::SetAxisLock(PhysicsEngine::PhysicsAixsLock physicsAxisLock)
{
	PhysicsManager::Instance().SetAxisLock(m_collider.lock().get(), physicsAxisLock);
}

void TLGameEngine::Rigidbody::SetGravity(bool flag)
{
	PhysicsManager::Instance().SetGravity(m_collider.lock().get(), flag);
}

void TLGameEngine::Rigidbody::SetKinematic(bool flag)
{
	PhysicsManager::Instance().SetKinematic(m_collider.lock().get(), flag);
}

void TLGameEngine::Rigidbody::UpdateRigidBodySetting()
{
	m_collider = m_gameObject.lock()->GetComponent<BoxCollider>();
	if (m_collider.expired())
	{
		m_collider = m_gameObject.lock()->GetComponent<SphereCollider>();
		if (m_collider.expired())
		{
			m_collider = m_gameObject.lock()->GetComponent<CapsuleCollider>();
			if (m_collider.expired())
			{
				m_collider = m_gameObject.lock()->GetComponent<MeshCollider>();
				if (m_collider.expired())
				{
					assert(false);
				}
			}
		}
	}

	PhysicsManager::Instance().SetAxisLock(m_collider.lock().get(), m_physicsAxisLock);
	PhysicsManager::Instance().SetMass(m_collider.lock().get(), m_Mass);
	PhysicsManager::Instance().SetKinematic(m_collider.lock().get(), m_IsKinematic);
	PhysicsManager::Instance().SetGravity(m_collider.lock().get(), m_UseGravity);
}

void TLGameEngine::Rigidbody::SetLinearVelocity(PhysicsEngine::Vector3 velocity)
{
	PhysicsManager::Instance().SetLinearVelocity(m_collider.lock().get(), velocity);
}

void TLGameEngine::Rigidbody::SetAngularVelocity(PhysicsEngine::Vector3 velocity)
{
	PhysicsManager::Instance().SetAngularVelocity(m_collider.lock().get(), velocity);
}

DirectX::SimpleMath::Vector3 TLGameEngine::Rigidbody::GetAngularVelocity()
{
	return PhysicsManager::Instance().GetAngularVelocity(m_collider.lock().get());
}

DirectX::SimpleMath::Vector3 TLGameEngine::Rigidbody::GetLinearVelocity()
{
	return PhysicsManager::Instance().GetAngularVelocity(m_collider.lock().get());
}

void TLGameEngine::Rigidbody::SetPhysicsAxisLock(bool linearX, bool linearY, bool linearZ, bool angularX, bool angularY, bool angularZ)
{
	m_physicsAxisLock = { linearX, linearY, linearZ, angularX, angularY, angularZ };
	if (!m_collider.expired())
	{
		SetAxisLock(m_physicsAxisLock);
	}
}

void TLGameEngine::Rigidbody::SetMaxLinearVelocity(float vel)
{
	PhysicsManager::Instance().SetMaxLinearVelocity(m_collider.lock().get(), vel);
}

void TLGameEngine::Rigidbody::SetMaxAngularVelocity(float vel)
{
	PhysicsManager::Instance().SetMaxAngularVelocity(m_collider.lock().get(), vel);
}

void TLGameEngine::Rigidbody::Start()
{
}

void TLGameEngine::Rigidbody::Update()
{
}

void TLGameEngine::Rigidbody::LateUpdate()
{
}

void TLGameEngine::Rigidbody::FixedUpdate()
{
}

void TLGameEngine::Rigidbody::OnDestroy()
{
}

