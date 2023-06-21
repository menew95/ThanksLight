#include "pch.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "IGraphicsEngine.h"
#include "ResourceManager.h"
#include "MeshFilter.h"

TLGameEngine::MeshRenderer::MeshRenderer()
{
}

void TLGameEngine::MeshRenderer::Awake()
{
	m_transform = GetTransform();

	//test code
	//AddMaterial(ResourceManager::Instance().CreateMaterial("aas"));

	std::shared_ptr<MeshFilter> meshFilter = m_gameObject.lock()->GetComponent<MeshFilter>();
	assert(meshFilter != nullptr);
	
	SetMesh(ResourceManager::Instance().GetMesh(meshFilter->GetMeshUID()));
}

void TLGameEngine::MeshRenderer::LateUpdate()
{
	m_data.world = m_transform->GetWorldTM();
	m_data.worldView = m_data.world * Camera::GetMainCamera()->GetViewMatrix();
	m_data.worldViewProj = m_data.worldView * Camera::GetMainCamera()->GetProjectionMatrix();
	m_data.viewPosition = Camera::GetMainCamera()->GetTransform()->GetWorldPosition();

	std::vector<TLGraphicsEngine::IMaterial*> _mats;

	for (auto mat : m_materials)
	{
		_mats.push_back(mat->GetIMaterial().get());
	}
#ifdef _DEBUG
	if (_mats.size() == 0)
	{
		assert(false);
	}
		
#endif // _DEBUG


 	if(m_mesh->GetIMesh() != nullptr)
		GameEngine::Instance().GetGraphicsEngine()->RegistRenderQueue(m_mesh->GetIMesh().get(), _mats, &m_data.world._11, 0);

	//Renderer::Instance().GetRenderer()->SetConstantBuffer(CONSTANT_BUFFER_TYPE::Transform, &_data, sizeof(_data), m_gameObject.lock().lock()->GetLayer().GetValue());

	//Renderer::Instance().GetRenderer()->DrawMesh(_mesh->GetUID(), m_material->GetUID(), m_gameObject.lock().lock()->GetLayer().GetValue());
}
