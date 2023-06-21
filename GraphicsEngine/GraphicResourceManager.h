#pragma once
#include <vector>
#include <string>
#include <unordered_map>

/// <summary>
/// DirectX11 에서 사용될 리소스 컨테이너
/// </summary>

//#include "ResourceBase.h"
#include <typeinfo>
#include "DebugMesh.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

struct _finddata64i32_t;
namespace ASEParse
{
	class ASEParser;
	struct GeometryObject;
}
namespace MaterialParse
{
	class MaterialParser;
}

template<class ResourceType>
using Enable = typename std::enable_if<std::is_base_of<TLGraphicsEngine::ResourceBase, ResourceType>::value, bool>::type;

struct IPrefabInfo;

namespace TLGraphicsEngine
{	

	class Mesh;
	class DebugMesh;
	class Material;
	class Shader;
	//class Sprite;
	class Texture;
	//class ResourceBase;

	class GraphicResourceManager
	{
	public:
		GraphicResourceManager();
		virtual ~GraphicResourceManager();

		static GraphicResourceManager* Instance();
	private:
		static GraphicResourceManager* m_Instance;

		std::vector<shared_ptr<Mesh>> m_DebugMeshList;

		std::unordered_map<std::string, shared_ptr<Mesh>> m_MeshMap;
		std::unordered_map<std::string, shared_ptr<Material>> m_MaterialMap;
		std::unordered_map<std::string, shared_ptr<Shader>> m_ShaderMap;
		std::unordered_map<std::string, shared_ptr<Texture>> m_TextureMap;

		std::unordered_map<std::string, std::string> m_MeshUIDMap;
		std::unordered_map<std::string, std::string> m_MaterialUIDMap;
		std::unordered_map<std::string, std::string> m_ShaderUIDMap;
		std::unordered_map<std::string, std::string> m_TextureUIDMap;
	public:
		void Initialize();

		Mesh* GetMesh(const char* name);
		Material* GetMaterial(const char* name);
		Texture* GetTexture(const char* name);
		Shader* GetShader(const char* name);
		shared_ptr<Mesh> GetMeshRef(const char* name);
		shared_ptr<Material> GetMaterialRef(const char* name);
		shared_ptr<Texture> GetTextureRef(const char* name);
		shared_ptr<Shader> GetShaderRef(const char* name);
		
		shared_ptr<Mesh> CreateMesh(const char* name, UID uid);
		shared_ptr<Material> CreateMaterial(const char* name, UID uid);


		shared_ptr<Texture> CreateTexture(const char* name, UID uid);
		shared_ptr<Shader> CreateShader(const char* name, UID uid);




		Mesh* GetDebugMesh(int id);

		Mesh* GetMesh(UID id);
		Material* GetMaterial(UID id);
		Texture* GetTexture(UID id);
		Shader* GetShader(UID id);
		shared_ptr<Mesh> GetMeshRef(UID id);
		shared_ptr<Material> GetMaterialRef(UID id);
		shared_ptr<Texture> GetTextureRef(UID id);
		shared_ptr<Shader> GetShaderRef(UID id);

		Mesh* RegistMeshBuffer(std::string& name, Mesh* mesh);
		Material* RegistMaterialBuffer(const char* name, Material* material);



	private:
		void MakeDebugMesh();

		void FindAllFileInDir(std::vector<std::string>& paths, const std::string& searching, _finddata64i32_t& fd);
		
	public:
		const std::string meshPath = "Resource\\ASE\\";
		const std::string meshFilter = "*.txt";
		
		shared_ptr<Mesh> LoadMesh(const char* fileName);

	public:
		const std::string texturePath = "Resource\\Texture\\";
		const std::string textureFilter = "*.dds";
		
		shared_ptr<Texture> LoadTexture(const char* fileName);

	public:
		const std::string shaderPath = "Resource\\Shader\\";
		const std::string shaderFilter = "*.cso";

		shared_ptr<Shader> LoadShader(const char* fileName);

	public:
		MaterialParse::MaterialParser* matParser = nullptr;

		const std::string materialPath = "Resource\\Material\\";
		const std::string materialFilter = "*.mat";
		
		shared_ptr<Material> LoadMaterial(const char* fileName);

	public:
		ASEParse::ASEParser* aseParser = nullptr;

		const std::string asePath = "Resource\\ASE\\";
		const std::string aseFilter = "*.ase";
		
		void LoadASE(const std::string& path/*, IPrefabInfo& prefabData*/);

	public:
		//template<class ResourceType>
		//shared_ptr<ResourceType> GetResource(const char* name);

		template<class ResourceType, Enable<ResourceType>>
		shared_ptr<ResourceType> Load(const char* fileName);

		void ReleaseResource(eResourceType type, std::string key);
	};

//	template<class ResourceType>
//	shared_ptr<ResourceType>
//		GraphicResourceManager::GetResource(const char* name)
//	{
//		const type_info& resourceType = typeid(ResourceType);
//
//		shared_ptr<ResourceBase> resource;
//
//		if (resourceType == typeid(Mesh))
//		{
//			resource = GetMeshRef(name);
//		}
//		else if (resourceType == typeid(Material))
//		{
//			resource = GetMaterialRef(name);
//		}
//		else if (resourceType == typeid(Shader))
//		{
//			resource = GetShaderRef(name);
//		}
//		else if (resourceType == typeid(Texture))
//		{
//			resource = GetTextureRef(name);
//		}
//		else
//		{
//#ifdef _DEBUG
//			OutputDebugString(_T("Load unexpected type file.\n"));
//			assert(false);
//#endif // _DEBUG
//			return nullptr;
//		}
//
//		return resource;
//	}

	template<class ResourceType, Enable<ResourceType>>
	shared_ptr<ResourceType> GraphicResourceManager::Load(const char* fileName)
	{
		std::type_info resourceType = typeid(ResourceType);

		ResourceBase* resource;

		if (resourceType == typeid(Mesh))
		{
			resource = LoadMesh(fileName);
		}
		else if (resourceType == typeid(Material))
		{
			resource = LoadMaterial(fileName);
		}
		else if (resourceType == typeid(Texture))
		{
			resource = LoadShader(fileName);
		}
		else if (resourceType == typeid(Shader))
		{
			resource = LoadTexture(fileName);
		}
		else
		{
#ifdef _DEBUG
			OutputDebugString(_T("Load unexpected type file.\n"));
			assert(false);
#endif // _DEBUG
			return nullptr;
		}

		return shared_ptr<ResourceType>(reinterpret_cast<ResourceType*>(resource));
	}

}

