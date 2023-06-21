#include "pch.h"
#include "SceneManager.h"

#include <algorithm>
#include <stack>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <filesystem>

#include "DontDestroyScene.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Factory.h"
#include "MeshRenderer.h"
#include "GameEngine.h"
#include "PhysicsManager.h"
#include "..\GraphicsEngine\Export\IGraphicsEngine.h"

#include "boost/mp11.hpp"
#include "boost/describe.hpp"
#include "boost/json.hpp"

using namespace boost::describe;
using namespace boost;

void TLGameEngine::SceneManager::ChangeScene(int sceneNum)
{
	UnloadScene(m_currScene);

	LoadScene(sceneNum);
}

void TLGameEngine::SceneManager::ChangeScene(std::string sceneName)
{
	UnloadScene(m_currScene);

	LoadScene(sceneName);
}

void TLGameEngine::SceneManager::LoadScene(std::string sceneName)
{
	LoadSceneFromUnityScene("Scenes/" + sceneName + ".json");
}

void TLGameEngine::SceneManager::LoadScene(int sceneNum)
{
	if (m_scenesName.size() == 0)
	{
		LoadScenesName("Scenes/SceneLoadData.dat");
	}

	assert(m_scenesName.size() > 0 && "씬 로드 실패");

	LoadSceneFromUnityScene("Scenes/" + m_scenesName[sceneNum]);
	m_currentSceneIndex = sceneNum;
}

void TLGameEngine::SceneManager::UnloadScene(std::shared_ptr<Scene> scene)
{
	const std::vector<std::shared_ptr<GameObject>> gameObjects = scene->GetGameObjects();
	for (auto obj : gameObjects)
	{
		for (auto com : obj->GetComponents())
		{
			ComponentManager::Instance().RemoveComponent(com);
		}
	}

	m_unloadScene = scene;
}
 
void TLGameEngine::SceneManager::UnloadScene(std::string name)
{
	for (auto scene : m_loadedScene)
	{
		if (scene->GetName() == name)
		{
			UnloadScene(scene);
			break;
		}
	}
}

void TLGameEngine::SceneManager::UnloadScene(int sceneNum)
{
	for (auto scene : m_loadedScene)
	{
		if (scene->GetName() == m_scenesName[sceneNum])
		{
			UnloadScene(scene);
			break;
		}
	}
}

std::shared_ptr<TLGameEngine::GameObject> TLGameEngine::SceneManager::FindObject(std::string Uid)
{
	for (auto scene : m_loadedScene)
	{
		std::shared_ptr<GameObject> gameObject = scene->GetGameObject(Uid);
		if (gameObject != nullptr)
		{
			return gameObject;
		}
	}
	return nullptr;
}

std::shared_ptr<TLGameEngine::GameObject> TLGameEngine::SceneManager::FindObject(FileID fileId)
{
	return FindObject(fileId.fileID);
}

struct GameObjectData
{
	GameObjectData() = default;
	GameObjectData(std::shared_ptr<TLGameEngine::GameObject> gameObject)
	{
		this->gameObject = gameObject;
	}
	std::shared_ptr<TLGameEngine::GameObject> gameObject = nullptr;
	std::vector<std::string> componentUids = {};
};

void PrintJson(std::ostream& os, const json::value& jsonValue, std::string* indent = nullptr)
{
	std::string indent_;
	if (!indent)
		indent = &indent_;

	switch (jsonValue.kind())
	{
	case json::kind::object:
	{
		os << "{\n";
		indent->append(4, ' ');
		auto const& obj = jsonValue.get_object();
		if (!obj.empty())
		{
			auto it = obj.begin();
			while (true)
			{
				os << *indent << json::serialize(it->key()) << " : ";
				PrintJson(os, it->value(), indent);
				if (++it == obj.end())
					break;
				os << ",\n";
			}
		}

		os << "\n";
		indent->resize(indent->size() - 4);
		os << *indent << "}";
		break;
	}
	case json::kind::string:
	{
		os << json::serialize(jsonValue.get_string());
		break;
	}
	case json::kind::double_:
	{
		os << jsonValue.get_double();
		break;
	}
	case json::kind::int64:
	{
		os << jsonValue.get_int64();
		break;
	}
	case json::kind::uint64:
	{
		os << jsonValue.get_uint64();
		break;
	}
	default:
		break;
	}
}

void TLGameEngine::SceneManager::LoadScenesName(std::string filePath)
{
	std::fstream fs = std::fstream(filePath);
	std::string line;

	if (fs.is_open())
	{
		while (getline(fs, line))
		{
			m_scenesName.push_back(line);
		}

		fs.close();
	}
}

bool TLGameEngine::SceneManager::LoadSceneFromUnityScene(std::string fileName)
{
	std::fstream fs = std::fstream(fileName);
	std::string line;
	json::value jsonValue;

	if (fs.is_open())
	{
		try
		{
			json::error_code ec;
			json::stream_parser p;

			while (getline(fs, line))
			{
				p.write(line, ec);
				if (ec)
				{
					auto t = ec.what();
					assert(false && ("json parsing error : " + ec.what()).c_str());
				}
			}
			p.finish(ec);
			if (ec)
			{
				assert(false && ("json parsing error : " + ec.what()).c_str());
			}
			jsonValue = p.release();
		}
		catch (std::exception& e)
		{
			assert(false && ("json error" + std::string(e.what())).c_str());
			return false;
		}

		fs.close();
	}
	else
	{
		assert(false && "Scene File can't open");
		return false;
	}

	std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
	newScene->SetName(fileName.substr(fileName.find_first_of('/') + 1));
	std::unordered_map<std::string, GameObjectData> sceneGameObjects;
	std::unordered_map<std::string, std::shared_ptr<Component>> components;
	auto& objects = jsonValue.get_object();

	for (auto objectsIter = objects.begin(); objectsIter != objects.end(); ++objectsIter)
	{
		auto& member = objectsIter->value().get_object();

		if (member["className"] == "GameObject")
		{
			std::string uid = objectsIter->key().data();
			if (uid.find_first_of(' ') != uid.npos)
			{
				std::string prefabInstanceFileID = member["m_PrefabInstance"].as_object()["fileID"].get_string().c_str();
				uid = uid.substr(0, uid.find_first_of(' '));

				newScene->AddStrippedUID(prefabInstanceFileID, uid);
				continue;
			}
			std::shared_ptr<TLGameEngine::GameObject> gameObject = Factory::CreateObjectFromJson<GameObject>(objectsIter->value());
			newScene->AddGameObject(gameObject);
			gameObject->SetUID(uid);

			sceneGameObjects.insert(std::make_pair(uid, GameObjectData(gameObject)));
			json::array m_components = member["m_Component"].get_array();
			for (auto& com : m_components)
			{
				sceneGameObjects[uid].componentUids.push_back(com.get_object()["component"].get_object()["fileID"].get_string().data());
			}
		}
		else if (member["className"] == "PrefabInstance")
		{
			std::shared_ptr<TLGameEngine::GameObject> gameObject;
			std::string uid = objectsIter->key().data();

			gameObject = ResourceManager::Instance().LoadPrefab(member["guid"].get_string().c_str(), newScene);
			gameObject->SetUID(uid);


			std::shared_ptr<Transform> transform = gameObject->GetComponent<Transform>();
			Vector3 position = transform->GetLocalPosition();
			Quaternion rotation = transform->GetLocalRotateQ();
			Vector3 scale = transform->GetLocalScale();

			auto value = member["m_LocalPosition"].as_object()["x"];
			if (value.is_int64())
				position.x = (float)value.get_int64();
			else if (value.is_double())
				position.x = (float)value.get_double();

			value = member["m_LocalPosition"].as_object()["y"];
			if (value.is_int64())
				position.y = (float)value.get_int64();
			else if (value.is_double())
				position.y = (float)value.get_double();

			value = member["m_LocalPosition"].as_object()["z"];
			if (value.is_int64())
				position.z = (float)value.get_int64();
			else if (value.is_double())
				position.z = (float)value.get_double();

			//rotation
			value = member["m_LocalRotation"].as_object()["x"];
			if (value.is_int64())
				rotation.x = (float)value.get_int64();
			else if (value.is_double())
				rotation.x = (float)value.get_double();

			value = member["m_LocalRotation"].as_object()["y"];
			if (value.is_int64())
				rotation.y = (float)value.get_int64();
			else if (value.is_double())
				rotation.y = (float)value.get_double();

			value = member["m_LocalRotation"].as_object()["z"];
			if (value.is_int64())
				rotation.z = (float)value.get_int64();
			else if (value.is_double())
				rotation.z = (float)value.get_double();

			value = member["m_LocalRotation"].as_object()["w"];
			if (value.is_int64())
				rotation.w = (float)value.get_int64();
			else if (value.is_double())
				rotation.w = (float)value.get_double();

			//scale
			value = member["m_LocalScale"].as_object()["x"];
			if (value.is_int64())
				scale.x = (float)value.get_int64();
			else if (value.is_double())
				scale.x = (float)value.get_double();

			value = member["m_LocalScale"].as_object()["y"];
			if (value.is_int64())
				scale.y = (float)value.get_int64();
			else if (value.is_double())
				scale.y = (float)value.get_double();

			value = member["m_LocalScale"].as_object()["z"];
			if (value.is_int64())
				scale.z = (float)value.get_int64();
			else if (value.is_double())
				scale.z = (float)value.get_double();

			transform->SetLocalPosition(position);
			transform->SetLocalRotateQ(rotation);
			transform->SetLocalScale(scale);

			for (int i = 0; i < 7; i++)
			{
				std::string materialKey = "m_Materials" + std::to_string(i);
				value = member[materialKey];
				if (value != nullptr)
				{
					int index = (int)value.as_object()["index"].get_int64();
					std::string guid = value.as_object()["guid"].get_string().c_str();
					std::shared_ptr<MeshRenderer> meshRenderer = gameObject->GetComponent<MeshRenderer>();
					assert(meshRenderer != nullptr);

					meshRenderer->AddMaterial(ResourceManager::Instance().GetMaterial(guid));
				}
			}
		}
	}

	for (auto objectsIter = objects.begin(); objectsIter != objects.end(); ++objectsIter)
	{
		auto& member = objectsIter->value().get_object();

		if (member["className"] != "GameObject" && member["className"] != "PrefabInstance")
		{
			std::shared_ptr<Component> component;
			std::string uid = objectsIter->key().data();

			//stripped 일때 
			if (uid.find_first_of(' ') != uid.npos)
			{
				uid = uid.substr(0, uid.find_first_of(' '));

				std::string prefabFileID = member["m_PrefabInstance"].as_object()["fileID"].get_string().c_str();
				std::shared_ptr<Transform> transform = newScene->GetGameObject(prefabFileID)->GetComponent<Transform>();
				assert(transform != nullptr);

				auto& prefabTransformFileID = member["m_CorrespondingSourceObject"].as_object()["fileID"].get_string();

				if (transform->GetUID() != prefabTransformFileID.c_str())
				{
					std::function<bool(const std::shared_ptr<Transform>)> findTransform;
					findTransform = [&transform, &uid, &prefabTransformFileID, &findTransform](const std::shared_ptr<Transform> _transform) {
						if (_transform->GetUID() == prefabTransformFileID.c_str())
						{
							transform = _transform;
							return false;
						}

						auto& children = _transform->GetChildren();
						std::all_of(children.begin(), children.end(), findTransform);
						return true;
					};
					auto& children = transform->GetChildren();
					std::all_of(children.begin(), children.end(), findTransform);
				}

				component = transform;
			}
			else
			{
				component = Factory::CreateComponentFromJson(objectsIter->value());

				std::shared_ptr<Transform> transform = std::dynamic_pointer_cast<Transform>(component);
				if (transform != nullptr)
				{
					transform->UpdateMatrix();
				}
			}
			if (component != nullptr)
			{
				component->SetUID(uid);
				components.insert(std::make_pair(uid, component));
			}
		}
	}

	//SetComponents
	for (auto gameObjectDataPair : sceneGameObjects)
	{
		for (auto uid : gameObjectDataPair.second.componentUids)
		{
			if (components.find(uid) != components.end())
			{
				gameObjectDataPair.second.gameObject->AddComponent(components[uid]);
			}
		}
	}

	//Set Transform Hierarcy
	for (auto& pair_uid_com : components)
	{
		std::shared_ptr<Transform> transform = std::dynamic_pointer_cast<Transform>(pair_uid_com.second);

		if (transform != nullptr)
		{
			for (auto& fildID : transform->GetChildrenID())
			{
				if (components.find(fildID.fileID) != components.end())
				{
					std::shared_ptr<Transform> childTransform = std::static_pointer_cast<Transform>(components[fildID.fileID]);
					childTransform->InitParent(transform);
				}
			}
		}
	}

	newScene->Load();
	m_loadedScene.push_back(newScene);

	m_currScene = newScene;
	return true;
}

std::shared_ptr<TLGameEngine::Scene> TLGameEngine::SceneManager::GetScene()
{
	if (m_loadedScene.size() > 0)
	{
		return m_loadedScene[0];
	}
}

void TLGameEngine::SceneManager::LoadDontDestroyScene()
{
	std::shared_ptr<DontDestroyScene> _scene = std::make_shared<DontDestroyScene>();

	m_loadedScene.push_back(_scene);

	_scene->Load();
}

void TLGameEngine::SceneManager::Clear()
{
	if (m_unloadScene != nullptr)
	{
		m_unloadScene->Unload();

		auto iter = find(m_loadedScene.begin(), m_loadedScene.end(), m_unloadScene);

		assert(iter != m_loadedScene.end());

		m_loadedScene.erase(iter);

		m_unloadScene.reset();

		GameEngine::Instance().GetGraphicsEngine()->ClearLight();
		PhysicsManager::Instance().InitPhysics();


	}
}