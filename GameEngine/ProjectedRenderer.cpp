#include "pch.h"
#include "ProjectedRenderer.h"
#include "MeshRenderer.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "IGraphicsEngine.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "SceneManager.h"

TLGameEngine::ProjectedRenderer::ProjectedRenderer()
{

}

void TLGameEngine::ProjectedRenderer::Awake()
{
	m_transform = GetTransform();

	std::shared_ptr<MeshFilter> meshFilter = m_gameObject.lock()->GetComponent<MeshFilter>();
	assert(meshFilter != nullptr);

	SetMesh(ResourceManager::Instance().GetMesh(meshFilter->GetMeshUID()));


	for (auto fileInfo : m_materialIDs)
	{
		m_materials.push_back(ResourceManager::Instance().GetMaterial(fileInfo.guid));
	}
	//m_collider = SceneManager::Instance().FindObject(m_colliderID)->GetComponent<BoxCollider>();
}

void TLGameEngine::ProjectedRenderer::LateUpdate()
{
	if (!m_isRender2D)
	{
		auto transform = m_transform.lock();

		m_data.world = transform->GetWorldTM();
		m_data.worldView = m_data.world * Camera::GetMainCamera()->GetViewMatrix();
		m_data.worldViewProj = m_data.worldView * Camera::GetMainCamera()->GetProjectionMatrix();
		m_data.viewPosition = Camera::GetMainCamera()->GetTransform()->GetWorldPosition();

		std::vector<TLGraphicsEngine::IMaterial*> _mats;
		for (auto& mat : m_materials)
		{
			_mats.push_back(mat->GetIMaterial().get());
		}

		GameEngine::Instance().GetGraphicsEngine()->RegistRenderQueue(m_mesh->GetIMesh().get(), _mats, &m_data.world._11, 0);
	}
	else
	{
		if (!m_is2D)
		{
			auto transform = m_transform.lock();

			m_data.world = transform->GetWorldTM();
			m_data.worldView = m_data.world * Camera::GetMainCamera()->GetViewMatrix();
			m_data.worldViewProj = m_data.worldView * Camera::GetMainCamera()->GetProjectionMatrix();
			m_data.viewPosition = Camera::GetMainCamera()->GetTransform()->GetWorldPosition();

			std::vector<TLGraphicsEngine::IMaterial*> _mats;

			for (auto& mat : m_materials)
			{
				_mats.push_back(mat->GetIMaterial().get());
			}

			GameEngine::Instance().GetGraphicsEngine()->RegistRenderQueue(m_mesh->GetIMesh().get(), _mats, &m_data.world._11, 2);

			m_is2D = true;
		}
		else
		{
			m_is2D = true;
		}
	}
}

void TLGameEngine::ProjectedRenderer::SetRender2D()
{
	m_isRender2D = true;
	//m_collider.lock()->SetIsActive(false);
}
