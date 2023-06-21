#include "pch.h"
#include "Sprite.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"
#include "ResourceManager.h"

TLGameEngine::Sprite::Sprite()
{
}

void TLGameEngine::Sprite::Awake()
{
	//m_transform = GetTransform();

	//CreateMesh();
}

void TLGameEngine::Sprite::LateUpdate()
{
	//m_data.world = m_transform->GetWorldTM() * Camera::GetMainCamera()->GetTransform()->GetWorldTM();
	//m_data.worldView = m_data.world * Camera::GetMainCamera()->GetViewMatrix();
	//m_data.worldViewProj = m_data.worldView * Camera::GetMainCamera()->GetProjectionMatrix();
	//m_data.viewPosition = Camera::GetMainCamera()->GetTransform()->GetPosition();

	//Renderer::Instance().GetRenderer()->SetConstantBuffer(CONSTANT_BUFFER_TYPE::Transform, &_data, sizeof(_data), m_gameObject.lock().lock()->GetLayer().GetValue());

	//Renderer::Instance().GetRenderer()->DrawMesh(_mesh->GetUID(), m_material->GetUID(), m_gameObject.lock().lock()->GetLayer().GetValue());
}

void TLGameEngine::Sprite::CreateMesh()
{
	//_mesh = ResourceManager::Instance().CreateMesh();
	//float w2 = 0.5f;
	//float h2 = 0.5f;
	//float d2 = 0.5f;
	//std::vector<Vertex> vertices(4);

	//vertices[0] = Vertex{ Vector3(-w2, -h2, -d2), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	//vertices[1] = Vertex{ Vector3(-w2, +h2, -d2), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	//vertices[2] = Vertex{ Vector3(+w2, +h2, -d2), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	//vertices[3] = Vertex{ Vector3(+w2, -h2, -d2), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	//_mesh->SetVertices(vertices);

	//std::vector<size_t> indices(6);

	//indices[0] = 0; indices[1] = 1; indices[2] = 2;
	//indices[3] = 0; indices[4] = 2; indices[5] = 3;
	//_mesh->SetIndeices(indices);
}
