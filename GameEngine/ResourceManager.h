#pragma once

#include "Singleton.h"
#include "IGraphicsEngine.h"

#include "Vertex.h"

namespace TLGameEngine
{
class Mesh;
class Texture;
class Material;
class Shader;
class GameObject;
class MeshFilter;
class Scene;

class ResourceManager : public Singleton<ResourceManager>
{
public:
	void Init();

	std::shared_ptr<TLGameEngine::GameObject> GetCopyOfPrefab(const std::string& guid);

	std::shared_ptr<Mesh> CreateMesh(std::string filename, const UID& uid);
	std::shared_ptr<Mesh> CreateMesh(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices);
	
	std::shared_ptr<Mesh>		GetMesh(const UID& uid);
	std::shared_ptr<Material>	GetMaterial(const UID& uid);
	std::shared_ptr<Texture>	GetTexture(const UID& uid);
	std::shared_ptr<Shader>		GetShader(const UID& uid);
	std::string					GetScriptName(const UID& uid) { return m_script_Uid_FileName[uid]; }
	std::string					GetClipName(const UID& uid);

	std::shared_ptr<TLGameEngine::GameObject> LoadPrefab(const std::string& uid, std::shared_ptr<Scene> scene);
private:
	void LoadFileNameData(const std::string& fileName, std::unordered_map<std::string, std::string>& UID_Filename);
	std::shared_ptr<Texture> LoadTexture(const std::string& path, const UID& uid);
	std::shared_ptr<Shader> LoadShader(const std::string& path, const UID& uid);
	std::shared_ptr<Material> LoadMaterial(const UID uid);

	TLGraphicsEngine::IGraphicsEngine* m_renderer;

	std::unordered_map<UID, std::shared_ptr<Mesh>> m_meshes;
	std::unordered_map<UID, std::shared_ptr<Texture>> m_textures;
	std::unordered_map<UID, std::shared_ptr<Material>> m_materials;
	//std::unordered_map<UID, std::shared_ptr<GameObject>> m_prefabs;
	std::unordered_map<UID, std::shared_ptr<Shader>> m_shaders;
	
	std::unordered_map<std::string, std::string> m_texture_Uid_FileName;
	std::unordered_map<std::string, std::string> m_material_Uid_FileName;
	std::unordered_map<std::string, std::string> m_prefab_Uid_FileName;
	std::unordered_map<std::string, std::string> m_shader_Uid_FileName;
	std::unordered_map<std::string, std::string> m_mesh_Uid_FileName;
	std::unordered_map<std::string, std::string> m_audio_Uid_FileName;
	std::unordered_map<std::string, std::string> m_script_Uid_FileName;


	//임시코드 
	//std::vector<std::shared_ptr<GameObject>> m_allPrefabObjects;
};
}

