#include "pch.h"
#include "Level9Event.h"
#include "InteractiveObject.h"
#include "BoxCollider.h"
#include "MeshRenderer.h"
#include "Transform.h"

#include "SceneManager.h"
#include "ResourceManager.h"


TLGameEngine::Level9Event::Level9Event():
	Component(Component::Type::Action)
{

}

void TLGameEngine::Level9Event::Awake()
{
	m_FitfallObj = SceneManager::Instance().FindObject(m_FitfallObject)->GetComponent<InteractiveObject>();

	for (auto& id : m_FitfallColliders)
	{
		auto foundObj = SceneManager::Instance().FindObject(id);
		auto foundCollider = foundObj->GetComponent<BoxCollider>();
		foundCollider->SetIsActive(false);
		m_colliders.emplace_back(foundCollider);
	}

	for (auto& id : m_FitfallWallMeshs)
	{
		auto foundObj = SceneManager::Instance().FindObject(id);
		m_wallMeshs.emplace_back(foundObj->GetComponent<MeshRenderer>());
	}

	m_WallMaterial = ResourceManager::Instance().GetMaterial(m_WallMaterialID.guid);
	m_FloorMaterial = ResourceManager::Instance().GetMaterial(m_FloorMaterialID.guid);

	m_wallFloor = SceneManager::Instance().FindObject(m_FitfallFloorMesh)->GetComponent<MeshRenderer>();
}

void TLGameEngine::Level9Event::Start()
{
	for (auto& obj : m_colliders)
	{
		obj.lock()->SetIsActive(false);
	}
}

void TLGameEngine::Level9Event::Update()
{
	if (!m_FitfallObj.lock()->GetIs2DObject())
	{
		Vector3 trashCan = {0.f , 5.f, 100.f};

		m_FitfallObj.lock()->GetTransform()->SetWorldPosition(trashCan);
		
		for (auto& obj : m_colliders)
		{
			obj.lock()->SetIsActive(true);
		}

		for (auto& obj : m_wallMeshs)
		{
			obj.lock()->SetMaterial(m_WallMaterial.lock(), 0);
			obj.lock()->SetMaterial(m_WallMaterial.lock(), 1);
		}

		m_wallFloor.lock()->SetMaterial(m_FloorMaterial.lock(), 0);
	}
}
