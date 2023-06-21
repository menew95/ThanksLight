#include "pch.h"
#include "OutLine.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "IGraphicsEngine.h"
#include "ResourceManager.h"
#include "MeshFilter.h"

TLGameEngine::OutLine::OutLine()
	: Component(Type::Rendering)
{

}

TLGameEngine::OutLine::~OutLine()
{

}

void TLGameEngine::OutLine::Awake()
{
	m_transform = GetTransform();

	std::shared_ptr<MeshFilter> meshFilter = m_gameObject.lock()->GetComponent<MeshFilter>();
	assert(meshFilter != nullptr);

	m_mesh = ResourceManager::Instance().GetMesh(meshFilter->GetMeshUID());

	m_isActive = false;
}

void TLGameEngine::OutLine::LateUpdate()
{
	auto world = m_transform->GetWorldTM();
	std::vector<TLGraphicsEngine::IMaterial*> empty;

	if (m_mesh->GetIMesh() != nullptr)
		GameEngine::Instance().GetGraphicsEngine()->RegistRenderQueue(m_mesh->GetIMesh().get(), empty, &world._11, 3);
}
