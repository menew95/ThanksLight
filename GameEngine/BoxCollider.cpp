#include "pch.h"
#include "BoxCollider.h"
#include "GameEngine.h"
#include "Transform.h"
#include "../GraphicsEngine/Export/IGraphicsEngine.h"

TLGameEngine::BoxCollider::BoxCollider():
	Collider()
{
	m_colliderType = PhysicsEngine::eGeometryType::BOX;
}

TLGameEngine::BoxCollider::BoxCollider(Vector3 center, Vector3 size):
	Collider(center),
	m_Size(size)
{
	m_colliderType = PhysicsEngine::eGeometryType::BOX;
}

TLGameEngine::BoxCollider::~BoxCollider()
{

}

void TLGameEngine::BoxCollider::OnDrawGizmo()
{
	float color[4] = { 1, 1, 1, 1 };

	Matrix world = GetTransform().lock()->GetWorldTM();

	Matrix _world = Matrix::CreateScale(m_Size * 0.5f) * world;

	GameEngine::Instance().GetGraphicsEngine()->RegistGizmo(4, &_world._11, &color[0]);
}

