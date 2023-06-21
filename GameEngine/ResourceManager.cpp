#include "pch.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "GameEngine.h"
#include "GameObject.h"
#include "Factory.h"
#include "Component.h"
#include "Transform.h"
#include "MaterialData.h"
#include "Scene.h"

#include <filesystem>
#include "boost/mp11.hpp"
#include "boost/describe.hpp"
#include "boost/json.hpp"

using namespace boost::describe;
using namespace boost;

void TLGameEngine::ResourceManager::Init()
{
	m_renderer = GameEngine::Instance().GetGraphicsEngine();
	//LoadTextureNameData("");
	LoadFileNameData("Prefabs/PrefabsData.dat", m_prefab_Uid_FileName);
	LoadFileNameData("Materials/MaterialsData.dat", m_material_Uid_FileName);
	LoadFileNameData("Textures/TexturesData.dat", m_texture_Uid_FileName);
	LoadFileNameData("MeshData.dat", m_mesh_Uid_FileName);
	LoadFileNameData("ShaderData.dat", m_shader_Uid_FileName);
	LoadFileNameData("AudioData.dat", m_audio_Uid_FileName);
	LoadFileNameData("ScriptData.dat", m_script_Uid_FileName);
}

std::shared_ptr<TLGameEngine::GameObject> TLGameEngine::ResourceManager::GetCopyOfPrefab(const std::string& guid)
{

	return nullptr;
}

std::shared_ptr<TLGameEngine::Mesh> TLGameEngine::ResourceManager::CreateMesh(std::string filename, const UID& uid)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	mesh->SetIMesh(GameEngine::Instance().GetGraphicsEngine()->CreateMesh(filename.c_str(), uid));
	//UID uid;
	//m_renderer->CreateMesh(&uid);
	//mesh->SetUID(uid);
	m_meshes.insert(std::make_pair(uid, mesh));

	return mesh;
}

std::shared_ptr<TLGameEngine::Mesh> TLGameEngine::ResourceManager::CreateMesh(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
{
	assert(false);
	return std::shared_ptr<Mesh>();
}

std::shared_ptr<TLGameEngine::Texture> TLGameEngine::ResourceManager::LoadTexture(const std::string& path, const UID& uid)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	//UID uid;
	texture->SetITexture(m_renderer->CreateTexture(path.c_str(), uid));
	//texture->SetUID(uid);
	m_textures.insert(std::make_pair(uid, texture));
	texture->SetUID(uid);

	return texture;
}

std::shared_ptr<TLGameEngine::Shader> TLGameEngine::ResourceManager::LoadShader(const std::string& path, const UID& uid)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();

	shader->SetIShader(m_renderer->CreateShader(path.c_str(), uid));

	m_shaders.insert(std::make_pair(uid, shader));
	//shader->SetUID(uid);

	return shader;
}

std::shared_ptr<TLGameEngine::Material> TLGameEngine::ResourceManager::LoadMaterial(const UID uid)
{
	assert(m_material_Uid_FileName.find(uid) != m_material_Uid_FileName.end());
	std::shared_ptr<Material> material = std::make_shared<Material>();

	if (m_material_Uid_FileName.find(uid) != m_material_Uid_FileName.end())
	{
		std::filesystem::path fileName = "Materials/" + m_material_Uid_FileName[uid];
		fileName.replace_extension(".json");
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
				return nullptr;
			}

			fs.close();
		}
		else
		{
			assert(false && "Scene File can't open");
			return nullptr;
		}

		auto& objects = jsonValue.get_object();

		float _metallic = 0, _smoothness = 0;

		for (auto objectsIter = objects.begin(); objectsIter != objects.end(); ++objectsIter)
		{
			auto& member = objectsIter->value().get_object();

			auto& properties = member["m_SavedProperties"];
			if (properties != nullptr)
			{
				MaterialParsingData materialData = json::value_to<MaterialParsingData>(objectsIter->value());
				auto iMaterial = GameEngine::Instance().GetGraphicsEngine()->CreateMaterial(materialData.m_Name.c_str(), uid);
				material->SetIMaterial(iMaterial);

				auto& textures = member["m_SavedProperties"].get_object()["m_TexEnvs"].get_array()[0];
				
				auto& mainTex = textures.get_object()["_MainTex"].get_object()["m_Texture"];
				if (mainTex.get_object()["guid"] != nullptr)
				{
					material->SetTexture(GetTexture(mainTex.get_object()["guid"].get_string().c_str()));
					
					auto& _Scale = textures.get_object()["_MainTex"].get_object()["m_Scale"];
					Vector2 tiling = { 1, 1 };
					if (_Scale != nullptr)
					{
						auto& x = _Scale.get_object()["x"];
						if (x.is_double())
						{
							tiling.x = (float)x.get_double();
						}
						else if (x.is_uint64())
						{
							tiling.x = (float)x.get_uint64();
						}
						else if (x.is_int64())
						{
							tiling.x = (float)x.get_int64();
						}

						auto& y = _Scale.get_object()["y"];
						if (y.is_double())
						{
							tiling.y = (float)y.get_double();
						}
						else if (y.is_uint64())
						{
							tiling.y = (float)y.get_uint64();
						}
						else if (y.is_int64())
						{
							tiling.y = (float)y.get_int64();
						}
					}

					material->SetTiling(tiling);
				}

				auto& bumTex = textures.get_object()["_BumpMap"].get_object()["m_Texture"];
				if (bumTex.get_object()["guid"] != nullptr)
				{
					material->SetBumpTexture(GetTexture(bumTex.get_object()["guid"].get_string().c_str()));

					/*auto& _Scale = textures.get_object()["_BumpMap"].get_object()["m_Scale"];
					Vector2 tiling = { 1, 1 };
					if (_Scale != nullptr)
					{
						auto& x = _Scale.get_object()["x"];
						if (x.is_double())
						{
							tiling.x = (float)x.get_double();
						}
						else if (x.is_uint64())
						{
							tiling.x = (float)x.get_uint64();
						}
						else if (x.is_int64())
						{
							tiling.x = (float)x.get_int64();
						}

						auto& y = _Scale.get_object()["y"];
						if (y.is_double())
						{
							tiling.y = (float)y.get_double();
						}
						else if (y.is_uint64())
						{
							tiling.y = (float)y.get_uint64();
						}
						else if (y.is_int64())
						{
							tiling.y = (float)y.get_int64();
						}
					}

					material->SetTiling(tiling);*/
				}

				auto& floats = member["m_SavedProperties"].get_object()["m_Floats"];

				for (auto& item : floats.get_array())
				{
					auto& valueMetalic = item.get_object()["_Metallic"];
					if (valueMetalic != nullptr)
					{
						if (valueMetalic.is_double())
						{
							_metallic = (float)valueMetalic.get_double();
						}
						else if (valueMetalic.is_uint64())
						{
							_metallic = (float)valueMetalic.get_uint64();
						}
						else if (valueMetalic.is_int64())
						{
							_metallic = (float)valueMetalic.get_int64();
						}
					}

					auto& valueSmoothness = item.get_object()["_Glossiness"];
					if (valueSmoothness != nullptr)
					{
						if (valueSmoothness.is_double())
						{
							_smoothness = (float)valueSmoothness.get_double();
						}
						else if (valueSmoothness.is_uint64())
						{
							_smoothness = (float)valueSmoothness.get_uint64();
						}
						else if (valueSmoothness.is_int64())
						{
							_smoothness = (float)valueSmoothness.get_int64();
						}
					}
				}
				
				auto& colors = member["m_SavedProperties"].get_object()["m_Colors"];

				Color _color, _emissionColor;

				for (auto& item : colors.get_array())
				{
					auto& valueColor = item.get_object()["_Color"];
					if (valueColor != nullptr)
					{
						auto& colorR = valueColor.get_object()["r"];
						if (colorR.is_double())
						{
							_color.x = (float)colorR.get_double();
						}
						else if (colorR.is_uint64())
						{
							_color.x = (float)colorR.get_uint64();
						}
						else if (colorR.is_int64())
						{
							_color.x = (float)colorR.get_int64();
						}

						auto& colorG = valueColor.get_object()["g"];
						if (colorG.is_double())
						{
							_color.y = (float)colorG.get_double();
						}
						else if (colorG.is_uint64())
						{
							_color.y = (float)colorG.get_uint64();
						}
						else if (colorG.is_int64())
						{
							_color.y = (float)colorG.get_int64();
						}

						auto& colorB = valueColor.get_object()["b"];
						if (colorG.is_double())
						{
							_color.z = (float)colorG.get_double();
						}
						else if (colorG.is_uint64())
						{
							_color.z = (float)colorG.get_uint64();
						}
						else if (colorG.is_int64())
						{
							_color.z = (float)colorG.get_int64();
						}

						auto& colorA = valueColor.get_object()["a"];
						if (colorA.is_double())
						{
							_color.w = (float)colorA.get_double();
						}
						else if (colorA.is_uint64())
						{
							_color.w = (float)colorA.get_uint64();
						}
						else if (colorA.is_int64())
						{
							_color.w = (float)colorA.get_int64();
						}

					}

					auto& valueEmission = item.get_object()["_EmissionColor"];
					if (valueEmission != nullptr)
					{
						auto& emissionColorR = valueEmission.get_object()["r"];
						if (emissionColorR.is_double())
						{
							_emissionColor.x = (float)emissionColorR.get_double();
						}
						else if (emissionColorR.is_uint64())
						{
							_emissionColor.x = (float)emissionColorR.get_uint64();
						}
						else if (emissionColorR.is_int64())
						{
							_emissionColor.x = (float)emissionColorR.get_int64();
						}

						auto& emissionColorG = valueEmission.get_object()["g"];
						if (emissionColorG.is_double())
						{
							_emissionColor.y = (float)emissionColorG.get_double();
						}
						else if (emissionColorG.is_uint64())
						{
							_emissionColor.y = (float)emissionColorG.get_uint64();
						}
						else if (emissionColorG.is_int64())
						{
							_emissionColor.y = (float)emissionColorG.get_int64();
						}

						auto& emissionColorB = valueEmission.get_object()["b"];
						if (emissionColorB.is_double())
						{
							_emissionColor.z = (float)emissionColorB.get_double();
						}
						else if (emissionColorB.is_uint64())
						{
							_emissionColor.z = (float)emissionColorB.get_uint64();
						}
						else if (emissionColorB.is_int64())
						{
							_emissionColor.z = (float)emissionColorB.get_int64();
						}

						auto& emissionColorA = valueEmission.get_object()["a"];
						if (emissionColorA.is_double())
						{
							_emissionColor.w = (float)emissionColorA.get_double();
						}
						else if (emissionColorA.is_uint64())
						{
							_emissionColor.w = (float)emissionColorA.get_uint64();
						}
						else if (emissionColorA.is_int64())
						{
							_emissionColor.w = (float)emissionColorA.get_int64();
						}
					}
				}

				material->SetMetallic(_metallic);
				material->SetSmoothness(_smoothness);
				material->SetColor(_color);
				material->SetEmissiveColor(_emissionColor);


				auto& _shaderGuid = member["m_Shader"].get_object()["guid"];

				if (_shaderGuid != nullptr)
				{
					std::string _shader = _shaderGuid.get_string().c_str();
					material->SetShader(GetShader(_shader));

				}
				else
				{
					// 쉐이더 uid 가 들어오지 않았음
					assert(false);
				}
			}
		}

		m_materials[uid] = material;
	}

	return material;
}

std::shared_ptr<TLGameEngine::Mesh> TLGameEngine::ResourceManager::GetMesh(const UID& uid)
{
	if (m_meshes.find(uid) == m_meshes.end())
	{
		if (m_mesh_Uid_FileName[uid] == "")
		{
			return nullptr;
		}
		else
		{
			return CreateMesh(m_mesh_Uid_FileName[uid], uid);
		}
	}
	return m_meshes[uid];
}

std::shared_ptr<TLGameEngine::Material> TLGameEngine::ResourceManager::GetMaterial(const UID& uid)
{
	//std::shared_ptr<Material> material = std::make_shared<Material>();

	//material->SetIMaterial(GameEngine::Instance().GetGraphicsEngine()->CreateMaterial("", uid));
	if (m_materials.find(uid) == m_materials.end())
	{
		if (m_material_Uid_FileName[uid] == "")
		{
			return nullptr;
		}
		else
		{
			return LoadMaterial(uid);
		}
	}
	return m_materials[uid];
}

std::shared_ptr<TLGameEngine::Texture> TLGameEngine::ResourceManager::GetTexture(const UID& uid)
{
	if (m_textures.find(uid) == m_textures.end())
	{
		if (m_texture_Uid_FileName[uid] == "")
		{
			return nullptr;
		}
		else
		{
			return LoadTexture(m_texture_Uid_FileName[uid], uid);
		}
	}
	return m_textures[uid];
}

std::shared_ptr<TLGameEngine::Shader> TLGameEngine::ResourceManager::GetShader(const UID& uid)
{
	if (m_shaders.find(uid) == m_shaders.end())
	{
		if (uid == "0000000000000000f000000000000000")
		{
			std::shared_ptr<Shader> shader = std::make_shared<Shader>();

			shader->SetIShader(m_renderer->GetShaderRef("Deferred_PS"));

			m_shaders.insert(std::make_pair(uid, shader));

			return shader;
		}
		else if(m_shader_Uid_FileName.find(uid) != m_shader_Uid_FileName.end())
		{
			auto shderName = m_shader_Uid_FileName[uid];

			std::shared_ptr<Shader> shader = std::make_shared<Shader>();

			shader->SetIShader(m_renderer->GetShaderRef(shderName.c_str()));

			m_shaders.insert(std::make_pair(uid, shader));
		}
		else
		{
			// 이상한 쉐이더가 설정됨
			assert(false);
		}
	}

	return m_shaders[uid];
}

std::string TLGameEngine::ResourceManager::GetClipName(const UID& uid)
{
	if (m_audio_Uid_FileName.find(uid) != m_audio_Uid_FileName.end())
	{
		return m_audio_Uid_FileName[uid];
	}
	assert(false);

	return nullptr;
}

std::shared_ptr<TLGameEngine::GameObject> TLGameEngine::ResourceManager::LoadPrefab(const std::string& uid, std::shared_ptr<Scene> scene)
{
	std::filesystem::path path = "Prefabs/" + m_prefab_Uid_FileName[uid];
	path.replace_extension("json");
	std::fstream fs = std::fstream(path);
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
		}

		fs.close();
	}
	else
	{
		assert(false && "Scene File can't open");
	}

	std::shared_ptr<GameObject> gameObject;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	auto& objects = jsonValue.get_object();
	std::vector<std::shared_ptr<Transform>> transforms;

	for (auto objectsIter = objects.begin(); objectsIter != objects.end(); ++objectsIter)
	{
		auto& member = objectsIter->value().get_object();

		if (member["className"] == "GameObject")
		{
			gameObject = Factory::CreateObjectFromJson<GameObject>(objectsIter->value());
			std::string uid = objectsIter->key().data();
			gameObject->SetUID(uid);
			gameObjects.push_back(gameObject);

			scene->AddGameObject(gameObject);
		}
		else
		{
			std::shared_ptr<Component> component = Factory::CreateComponentFromJson(objectsIter->value());
			if (component != nullptr)
			{
				std::string uid = objectsIter->key().data();
		

				component->SetUID(uid);

				gameObject->AddComponent(component);
			}

			if (member["className"] == "Transform")
			{
				transforms.push_back(std::static_pointer_cast<Transform>(component));
			}
		}
	}

	for (size_t i = 0; i < transforms.size(); i++)
	{
		for (size_t j = 0; j < transforms.size(); j++)
		{
			if (transforms[i]->GetFatherID().fileID == transforms[j]->GetUID())
			{
				transforms[i]->InitParent(transforms[j]);
			}
		}
	}

	//assert(m_prefabs.find(gameObject->GetUID()) == m_prefabs.end());

	//m_prefabs.insert(std::make_pair(gameObject->GetUID(), gameObject));

	return gameObject;
}

void TLGameEngine::ResourceManager::LoadFileNameData(const std::string& fileName, std::unordered_map<std::string, std::string>& UID_Filename)
{
	std::fstream fs = std::fstream(fileName);

	assert(fs.is_open());
	if (fs.is_open())
	{
		std::string UID;
		std::string fileName;

		while (getline(fs, fileName))
		{
			getline(fs, UID);

			assert(UID_Filename.find(UID) == UID_Filename.end());
			//std::wstring wFileName{ fileName.begin(), fileName.end() };
			UID_Filename.insert(std::make_pair(UID, fileName));
		}
	}
}
