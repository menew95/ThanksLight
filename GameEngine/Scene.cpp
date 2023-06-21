#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "GameEngine.h"
#include "SceneManager.h"
#include "ResourceManager.h"

#include "BoxCollider.h"
#include "Rigidbody.h"
#include "PlayerController.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraController.h"
#include "Mesh.h"
#include "Light.h"
#include "Material.h"
#include "Sprite.h"
#include "InteractiveObject.h"

// UI
#include "Canvas.h"
#include "Text.h"
#include "Image.h"
#include "RectTransform.h"
#include "DebugManager.h"
#include "PostProcess.h"


TLGameEngine::Scene::Scene()
{
}

TLGameEngine::Scene::~Scene()
{
}

void TLGameEngine::Scene::Load()
{
	ResourceManager& resourceManager = ResourceManager::Instance();


}

void TLGameEngine::Scene::Unload()
{

}

std::shared_ptr<TLGameEngine::GameObject> TLGameEngine::Scene::GetGameObject(std::string Uid)
{
	if (m_strippedUID_instanceUID.find(Uid) != m_strippedUID_instanceUID.end())
	{
		Uid = m_strippedUID_instanceUID[Uid];
	}
	for (auto obj : m_gameObjects)
	{
		if (obj->GetUID() == Uid)
		{
			return obj;
		}
	}

	return nullptr;
}

std::vector<std::shared_ptr<TLGameEngine::GameObject>>& TLGameEngine::Scene::GetGameObjects()
{
	return m_gameObjects;
}

std::string TLGameEngine::Scene::GetStrippedUID(std::string instanceUID)
{
	return m_strippedUID_instanceUID[instanceUID];
}

void TLGameEngine::Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
	m_gameObjects.push_back(gameObject);
}

void TLGameEngine::Scene::AddStrippedUID(std::string instanceUID, std::string strippedUID)
{
	if (m_strippedUID_instanceUID.find(strippedUID) == m_strippedUID_instanceUID.end())
	{
		m_strippedUID_instanceUID[strippedUID] = instanceUID;
	}
	else
	{
		assert(false);
	}
}
