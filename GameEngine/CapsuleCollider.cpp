#include "pch.h"
#include "CapsuleCollider.h"

TLGameEngine::CapsuleCollider::CapsuleCollider() :
	Collider()
{
	m_colliderType = PhysicsEngine::eGeometryType::CAPSULE;
}


TLGameEngine::CapsuleCollider::CapsuleCollider(Vector3 center, float radius, float halfHeight):
	Collider(center),
	m_Radius(radius),
	m_Height(halfHeight)
{
	m_colliderType = PhysicsEngine::eGeometryType::CAPSULE;
}


TLGameEngine::CapsuleCollider::~CapsuleCollider()
{

}
