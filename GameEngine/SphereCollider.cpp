#include "pch.h"
#include "SphereCollider.h"

TLGameEngine::SphereCollider::SphereCollider()
{
	m_colliderType = PhysicsEngine::eGeometryType::SPHERE;
}

TLGameEngine::SphereCollider::SphereCollider(Vector3 center, float radius)
	:Collider(center),
	m_Radius(radius)
{
	m_colliderType = PhysicsEngine::eGeometryType::SPHERE;
}

TLGameEngine::SphereCollider::~SphereCollider()
{

}
