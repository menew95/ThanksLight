#include "pch.h"
#include "MeshCollider.h"

TLGameEngine::MeshCollider::MeshCollider()
{
	m_colliderType = PhysicsEngine::eGeometryType::TRIANGLEMESH;
}

TLGameEngine::MeshCollider::~MeshCollider()
{

}

