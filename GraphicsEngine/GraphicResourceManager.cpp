#include "pch.h"
#include "GraphicResourceManager.h"
#include "GraphicsEngine.h"

#include "Mesh.h"
#include "DebugMesh.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

#include "Parser/ASEParser.h"
#include "Parser/MaterialParser.h"

/// <summary>
/// 임시 파서
/// </summary>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <iterator>
#include <windows.h>
#include <vector>
#include <stdexcept>
#include <io.h>

using namespace std;
using namespace TLGraphicsEngine;

GraphicResourceManager* GraphicResourceManager::m_Instance = nullptr;

vector<string> split(string input, char delimiter)
{
	vector<string> answer;
	stringstream ss(input);
	string temp;

	while (getline(ss, temp, delimiter))
	{
		answer.push_back(temp);
	}

	return answer;
}

GraphicResourceManager::GraphicResourceManager()
:
aseParser(nullptr),
matParser(nullptr)
{
	if (m_Instance == nullptr)
	{
		m_Instance = this;
	}
}

GraphicResourceManager::~GraphicResourceManager()
{
	if (aseParser != nullptr)
	{
		delete aseParser;
		aseParser = nullptr;
	}

	if (matParser != nullptr)
	{
		delete matParser;
		matParser = nullptr;
	}
} 

GraphicResourceManager* GraphicResourceManager::Instance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new GraphicResourceManager;
		m_Instance->Initialize();
	}

	return m_Instance;
}

void GraphicResourceManager::Initialize()
{
	if (aseParser == nullptr)
	{
		aseParser = new ASEParse::ASEParser;
		aseParser->Initialize();
	}
	if (matParser == nullptr)
	{
		matParser = new MaterialParse::MaterialParser;
		matParser->Initialize();
	}

	MakeDebugMesh();
}

Mesh* GraphicResourceManager::GetMesh(const char* name)
{
	std::string sName(name);

	auto mesh = m_MeshMap.find(sName);
	Mesh* loadMesh = nullptr;

	if (mesh != m_MeshMap.end())
	{
		loadMesh = mesh->second.get();
	}
	else
	{
		LoadASE(name);

		auto mesh = m_MeshMap.find(name);
		//shared_ptr<Mesh> loadMesh;

		if (mesh != m_MeshMap.end())
		{
			loadMesh = mesh->second.get();
		}
		else
		{
			// 매쉬 로드 실패
			assert(false);
		}
	}

	return loadMesh;
}

TLGraphicsEngine::Mesh* GraphicResourceManager::GetMesh(UID id)
{
	auto name = m_MeshUIDMap.find(id);

	if (name == m_MeshUIDMap.end())
	{
		assert(false);
	}

	auto mesh = m_MeshMap.find(name->second);

	if (mesh == m_MeshMap.end())
	{
		assert(false);
	}

	return mesh->second.get();
}

Mesh* GraphicResourceManager::GetDebugMesh(int id)
{
	if (id < (int)m_DebugMeshList.size())
	{
		return m_DebugMeshList[id].get();
	}

	return nullptr;
}

Mesh* GraphicResourceManager::RegistMeshBuffer(std::string& name, Mesh* mesh)
{
	shared_ptr<Mesh> _newMesh = std::shared_ptr<Mesh>(mesh);
	m_MeshMap.insert(make_pair(name, _newMesh));

	GraphicsEngine::Instance()->CreateMeshBuffer(_newMesh);

	return mesh;
}

Material* GraphicResourceManager::RegistMaterialBuffer(const char* name, Material* material)
{
	m_MaterialMap.insert(make_pair(name, material));

	shared_ptr<Shader> ps = GetShaderRef(material->m_PShaderID.c_str());

	if (!material->SetShader(ps))
	{
		delete material;
		return nullptr;
	}

	Texture* tex = nullptr;

	if (material->m_mainTexID != "None")
	{
		tex = GetTexture(material->m_mainTexID.c_str());
		if (tex == nullptr)
		{
			delete material;
			return nullptr;
		}

		material->m_mainTex = shared_ptr<Texture>(tex);
	}

	if (material->m_bumpTexID != "None")
	{
		tex = GetTexture(material->m_bumpTexID.c_str());
		if (tex == nullptr)
		{
			delete material;
			return nullptr;
		}

		material->m_bumpTex = shared_ptr<Texture>(tex);
	}

	if (material->m_metallicGlossMapID != "None")
	{
		tex = GetTexture(material->m_metallicGlossMapID.c_str());
		if (tex == nullptr)
		{
			delete material;
			return nullptr;
		}

		material->m_metallicGlossMap = shared_ptr<Texture>(tex);
	}

	if (material->m_occlusionMapID != "None")
	{
		tex = GetTexture(material->m_occlusionMapID.c_str());
		if (tex == nullptr)
		{
			delete material;
			return nullptr;
		}

		material->m_occlusionMap = shared_ptr<Texture>(tex);
	}

	return material;
}

Material* GraphicResourceManager::GetMaterial(const char* name)
{
	std::string sName(name);

	auto mat = m_MaterialMap.find(name);
	shared_ptr<Material> loadMat = nullptr;

	if (mat != m_MaterialMap.end())
	{
		loadMat = mat->second;
	}
	else
	{
		loadMat = LoadMaterial(name);
	}

	return loadMat.get();
}

TLGraphicsEngine::Material* GraphicResourceManager::GetMaterial(UID id)
{
	auto name = m_MaterialUIDMap.find(id);

	if (name == m_MaterialUIDMap.end())
	{
		assert(false);
	}

	auto material = m_MaterialMap.find(name->second);

	if (material == m_MaterialMap.end())
	{
		assert(false);
	}

	return material->second.get();
}

Texture* GraphicResourceManager::GetTexture(const char* name)
{
	std::string sName(name);

	auto tex = m_TextureMap.find(sName);
	shared_ptr<Texture> loadTex = nullptr;

	if (tex != m_TextureMap.end())
	{
		loadTex = tex->second;
	}
	else
	{
		loadTex = LoadTexture(name);
	}

	return loadTex.get();
}

TLGraphicsEngine::Texture* GraphicResourceManager::GetTexture(UID id)
{
	auto name = m_TextureUIDMap.find(id);

	if (name == m_TextureUIDMap.end())
	{
		assert(false);
	}

	auto texture = m_TextureMap.find(name->second);

	if (texture == m_TextureMap.end())
	{
		assert(false);
	}

	return texture->second.get();
}

Shader* GraphicResourceManager::GetShader(const char* name)
{
	std::string sName(name);
	
	auto shader = m_ShaderMap.find(name);
	shared_ptr<Shader> loadShader = nullptr;

	if (shader != m_ShaderMap.end())
	{
		loadShader = shader->second;
	}
	else
	{
		loadShader = LoadShader(name);
	}

	return loadShader.get();
}

TLGraphicsEngine::Shader* GraphicResourceManager::GetShader(UID id)
{
	auto name = m_ShaderUIDMap.find(id);

	if (name == m_ShaderUIDMap.end())
	{
		assert(false);
	}

	auto shader = m_ShaderMap.find(name->second);

	if (shader == m_ShaderMap.end())
	{
		assert(false);
	}

	return shader->second.get();
}

shared_ptr<Mesh> GraphicResourceManager::GetMeshRef(const char* name)
{
	std::string sName(name);

	auto mesh = m_MeshMap.find(name);
	shared_ptr<Mesh> loadMesh;

	if (mesh != m_MeshMap.end())
	{
		loadMesh = mesh->second;
	}
	else
	{
		LoadASE(name);

		auto mesh = m_MeshMap.find(name);

		if (mesh != m_MeshMap.end())
		{
			loadMesh = mesh->second;
		}
		else
		{
			// 매쉬 로드 실패
			//assert(false);
		}
	}

	return loadMesh;
	//return make_shared<Mesh>(loadMesh);
}

std::shared_ptr<TLGraphicsEngine::Mesh> GraphicResourceManager::GetMeshRef(UID id)
{
	auto name = m_MeshUIDMap.find(id);

	if(name == m_MeshUIDMap.end())
	{
		assert(false);
		return nullptr;
	}

	auto mesh = m_MeshMap.find(name->second);
	shared_ptr<Mesh> loadMesh;

	if (mesh != m_MeshMap.end())
	{
		loadMesh = mesh->second;
	}
	else
	{
		loadMesh = LoadMesh(name->second.c_str());
	}

	return loadMesh;
	//return make_shared<Mesh>(loadMesh);
}

shared_ptr<Material> GraphicResourceManager::GetMaterialRef(const char* name)
{
	auto material = m_MaterialMap.find(name);


	shared_ptr<Material> loadMat = nullptr;

	if (material != m_MaterialMap.end())
	{
		loadMat = material->second;
	}
	else
	{
		loadMat = LoadMaterial(name);
	}

	return loadMat;
}

std::shared_ptr<TLGraphicsEngine::Material> GraphicResourceManager::GetMaterialRef(UID id)
{
	auto name = m_MaterialUIDMap.find(id);

	if (name == m_MaterialUIDMap.end())
	{
		assert(false);
		return nullptr;
	}

	auto material = m_MaterialMap.find(name->second);
	shared_ptr<Material> loadMat = nullptr;

	if (material != m_MaterialMap.end())
	{
		loadMat = material->second;
	}
	else
	{
		loadMat = LoadMaterial(name->second.c_str());
	}

	return loadMat;
}

shared_ptr<Texture> GraphicResourceManager::GetTextureRef(const char* name)
{
	auto texture = m_TextureMap.find(name);
	shared_ptr<Texture> loadTex;

	if (texture != m_TextureMap.end())
	{
		loadTex = texture->second;
	}
	else
	{
		loadTex = LoadTexture(name);
	}

	return loadTex;
}

std::shared_ptr<TLGraphicsEngine::Texture> GraphicResourceManager::GetTextureRef(UID id)
{
	auto name = m_TextureUIDMap.find(id);

	if (name == m_TextureUIDMap.end())
	{
		assert(false);
		return nullptr;
	}

	auto texture = m_TextureMap.find(name->second);
	shared_ptr<Texture> loadTex = nullptr;

	if (texture != m_TextureMap.end())
	{
		loadTex = texture->second;
	}
	else
	{
		loadTex = LoadTexture(name->second.c_str());
	}

	return loadTex;
}

shared_ptr<Shader> GraphicResourceManager::GetShaderRef(const char* name)
{
	std::string sName(name);

	auto shader = m_ShaderMap.find(name);
	shared_ptr<Shader> loadShader = nullptr;

	if (shader != m_ShaderMap.end())
	{
		loadShader = shader->second;
	}
	else
	{
		loadShader = LoadShader(name);
	}

	return loadShader;
}

std::shared_ptr<TLGraphicsEngine::Shader> GraphicResourceManager::GetShaderRef(UID id)
{
	auto name = m_ShaderUIDMap.find(id);

	if (name == m_ShaderUIDMap.end())
	{
		assert(false);
		return nullptr;
	}

	auto shader = m_ShaderMap.find(name->second);
	shared_ptr<Shader> loadShader = nullptr;

	if (shader != m_ShaderMap.end())
	{
		loadShader = shader->second;
	}
	else
	{
		loadShader = LoadShader(name->second.c_str());
	}

	return loadShader;
}

void GraphicResourceManager::MakeDebugMesh()
{
	DebugMesh* cameraDebugMesh = new DebugMesh;
	cameraDebugMesh->MakeCameraMesh();
	m_DebugMeshList.push_back(shared_ptr<Mesh>(cameraDebugMesh));


	DebugMesh* girdDebugMesh = new DebugMesh;
	girdDebugMesh->MakeGridMesh();
	m_DebugMeshList.push_back(shared_ptr<Mesh>(girdDebugMesh));

	DebugMesh* axisDebugMesh = new DebugMesh;
	axisDebugMesh->MakeAxisMesh();
	m_DebugMeshList.push_back(shared_ptr<Mesh>(axisDebugMesh));

	DebugMesh* boundingBoxDebugMesh = new DebugMesh;
	boundingBoxDebugMesh->MakeBoundingBoxMesh();
	m_DebugMeshList.push_back(shared_ptr<Mesh>(boundingBoxDebugMesh));

	DebugMesh* boxCollider = new DebugMesh;
	boxCollider->MakeBoxColliderMesh();
	m_DebugMeshList.push_back(shared_ptr<Mesh>(boxCollider));

	for (auto& mesh : m_DebugMeshList)
	{
		GraphicsEngine::Instance()->CreateMeshBuffer(mesh);
		mesh->SetRenderstate(GraphicsEngine::Instance()->GetSolidRS());
	}
}

void GraphicResourceManager::FindAllFileInDir(std::vector<std::string>& paths, const std::string& searching, _finddata64i32_t& fd)
{
	intptr_t handle = _findfirst(searching.c_str(), &fd);  //현재 폴더 내 모든 파일을 찾는다.
	
	if (handle == -1)    return;

	int result = 0;
	do
	{
		paths.push_back(fd.name);
		result = _findnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
}

shared_ptr<Mesh> GraphicResourceManager::LoadMesh(const char* fileName)
{
	std::string path = meshPath + fileName + ".txt";

	ifstream file(path); // example.txt 파일을 연다. 없으면 생성. 
	bool hasUv = false;

	if (file.is_open())
	{
		shared_ptr<Mesh> newMesh = shared_ptr<Mesh>(new Mesh(fileName));
		int vertexCnt = 0, indexCnt = 0;
		bool vertex = false, index = false, vertexList = false, indexList = false;
		int cnt = 0;
		char c;
		string str;
		std::vector<Vertex> vers;
		std::vector<UINT> tris;

		Vertex tempVertex;
		int tempIndex = 0;
		int vertexType = 6;
		while (!file.eof())
		{
			c = file.get();
			if ((c == ' ' || c == '\t' || c == '\n') && !(indexList || vertexList))
			{
				if (vertex && vertexCnt == 0)
				{
					vertexCnt = stoi(str);
					vertex = false;
				}
				else if (index && indexCnt == 0)
				{
					indexCnt = stoi(str) * 3;
					index = false;
				}
				else if (str.compare("VertexCount:") == 0)
				{
					vertex = true;
				}
				else if (str.compare("TriangleCount:") == 0)
				{
					index = true;
				}
				else if (str.compare("VertexList") == 0)
				{
					vertexList = true;
					while (!file.eof())
					{
						c = file.get();
						str += c;
						if (c == 'u')
						{
							hasUv = true;
							vertexType = 8;
						}
						if (c == '{')
						{
							c = file.get();
							break;
						}
					}
				}
				else if (str.compare("TriangleList") == 0)
				{
					indexList = true;
					while (!file.eof())
					{
						c = file.get();
						if (c == '{')
						{
							c = file.get();
							break;
						}
					}
				}

				str.clear();
			}
			else if (c == '}')
			{
				vector<string> splitStr = split(str, ' ');
				
				if (vertexList)
				{
					int cnt = 0;
					for (auto& num : splitStr)
					{
						switch (cnt)
						{
							case 0:	// x
								tempVertex.Pos.x = stof(num);
								if (tempVertex.Pos.x < newMesh->m_MinBox.x)
								{
									newMesh->m_MinBox.x = tempVertex.Pos.x;
								}
								else if (tempVertex.Pos.x > newMesh->m_MaxBox.x)
								{
									newMesh->m_MaxBox.x = tempVertex.Pos.x;
								}
								break;
							case 1:	// y
								tempVertex.Pos.y = stof(num);
								if (tempVertex.Pos.y < newMesh->m_MinBox.y)
								{
									newMesh->m_MinBox.y = tempVertex.Pos.y;
								}
								else if (tempVertex.Pos.y > newMesh->m_MaxBox.y)
								{
									newMesh->m_MaxBox.y = tempVertex.Pos.x;
								}
								break;
							case 2:	// z
								tempVertex.Pos.z = stof(num);
								if (tempVertex.Pos.z < newMesh->m_MinBox.z)
								{
									newMesh->m_MinBox.z = tempVertex.Pos.z;
								}
								else if (tempVertex.Pos.z > newMesh->m_MaxBox.z)
								{
									newMesh->m_MaxBox.z = tempVertex.Pos.z;
								}
								break;
							case 3:	// z
								tempVertex.Normal.x = stof(num);
								break;
							case 4:	// z
								tempVertex.Normal.y = stof(num);
								break;
							case 5:	// z
								tempVertex.Normal.z = stof(num);
								break;
							case 6:	
								tempVertex.UV.x = stof(num);
								break;
							case 7:
								tempVertex.UV.y = stof(num);
								break;
						}

						cnt++;

						if (cnt == vertexType)
						{
							cnt = 0;
							vers.push_back(tempVertex);
						}
					}
					vertexList = false;
				}
				else if (indexList)
				{
					for (auto& num : splitStr)
					{
						tris.push_back((UINT)stoi(num));
					}
					indexList = false;
				}
			}
			else
			{
				if (indexList || vertexList)
				{
					if(c == '\n')
						c = ' ';
					else if(c == '\t')
						continue;
				}
				str += c;
			}
		
		}
		file.close(); // 열었떤 파일을 닫는다. 
		newMesh->SetVertices(vers);
		newMesh->SetTriangles(tris);


		if (newMesh->m_MinBox.x == 0 && newMesh->m_MinBox.y == 0 && newMesh->m_MinBox.z == 0 &&
			newMesh->m_MaxBox.x == 0 && newMesh->m_MaxBox.y == 0 && newMesh->m_MaxBox.z == 0)
		{
			
		}
		else
		{
			newMesh->m_BoundingBoxVertices.push_back(Vertex(DirectX::XMFLOAT3(newMesh->m_MinBox.x, newMesh->m_MinBox.y, newMesh->m_MinBox.z)));
			newMesh->m_BoundingBoxVertices.push_back(Vertex(DirectX::XMFLOAT3(newMesh->m_MinBox.x, newMesh->m_MaxBox.y, newMesh->m_MinBox.z)));
			newMesh->m_BoundingBoxVertices.push_back(Vertex(DirectX::XMFLOAT3(newMesh->m_MaxBox.x, newMesh->m_MaxBox.y, newMesh->m_MinBox.z)));
			newMesh->m_BoundingBoxVertices.push_back(Vertex(DirectX::XMFLOAT3(newMesh->m_MaxBox.x, newMesh->m_MinBox.y, newMesh->m_MinBox.z)));
			newMesh->m_BoundingBoxVertices.push_back(Vertex(DirectX::XMFLOAT3(newMesh->m_MinBox.x, newMesh->m_MinBox.y, newMesh->m_MaxBox.z)));
			newMesh->m_BoundingBoxVertices.push_back(Vertex(DirectX::XMFLOAT3(newMesh->m_MinBox.x, newMesh->m_MaxBox.y, newMesh->m_MaxBox.z)));
			newMesh->m_BoundingBoxVertices.push_back(Vertex(DirectX::XMFLOAT3(newMesh->m_MaxBox.x, newMesh->m_MaxBox.y, newMesh->m_MaxBox.z)));
			newMesh->m_BoundingBoxVertices.push_back(Vertex(DirectX::XMFLOAT3(newMesh->m_MaxBox.x, newMesh->m_MinBox.y, newMesh->m_MaxBox.z)));

			// Index
			UINT indTemp[] =
			{
				0, 1, 1, 2, 2, 3, 3, 0,
				4, 5, 5, 6, 6, 7, 7, 4,
				0, 4, 1, 5, 2, 6, 3, 7
			};

			for (auto index : indTemp)
			{
				newMesh->m_BoundingBoxTriangles.push_back(index);
			}
		}


		GraphicsEngine::Instance()->CreateMeshBuffer(newMesh);
		newMesh->SetRenderstate(GraphicsEngine::Instance()->GetSolidRS());

		m_MeshMap.insert(make_pair(fileName, shared_ptr<Mesh>(newMesh)));

		return newMesh;
	}
	
	return nullptr;
}

shared_ptr<Material> GraphicResourceManager::LoadMaterial(const char* fileName)
{
	std::string sPath = materialPath + fileName + ".mat";
	std::wstring wpath;
	wpath.assign(sPath.begin(), sPath.end());

	shared_ptr<Material> newMat = shared_ptr<Material>(new Material(fileName));

	LPSTR path = const_cast<char*>(sPath.c_str());
	
	if (!matParser->Load(path, *newMat))
	{
		return nullptr;
	}

	shared_ptr<Shader> ps = GetShaderRef(newMat->m_PShaderID.c_str());

	if (!newMat->SetShader(ps))
	{
		return nullptr;
	}


	shared_ptr<Texture> tex = nullptr;

	if (newMat->m_mainTexID != "None")
	{
		tex = GetTextureRef(newMat->m_mainTexID.c_str());
		if (tex == nullptr)
		{
			return nullptr;
		}

		newMat->m_mainTex = shared_ptr<Texture>(tex);
	}

	if (newMat->m_bumpTexID != "None")
	{
		tex = GetTextureRef(newMat->m_bumpTexID.c_str());
		if (tex == nullptr)
		{
			return nullptr;
		}

		newMat->m_bumpTex = tex;
	}

	if (newMat->m_metallicGlossMapID != "None")
	{
		tex = GetTextureRef(newMat->m_metallicGlossMapID.c_str());
		if (tex == nullptr)
		{
			return nullptr;
		}

		newMat->m_metallicGlossMap = tex;
	}

	if (newMat->m_occlusionMapID != "None")
	{
		tex = GetTextureRef(newMat->m_occlusionMapID.c_str());
		if (tex == nullptr)
		{
			return nullptr;
		}

		newMat->m_occlusionMap = tex;
	}

	m_MaterialMap.insert(make_pair(fileName, newMat));

	return newMat;
}

shared_ptr<Texture> GraphicResourceManager::LoadTexture(const char* fileName)
{
	std::string path = texturePath + fileName;

	shared_ptr<Texture> newTex = shared_ptr<Texture>(new Texture(fileName));

	if (!newTex->MakeTexture(path.c_str()))
	{
		return nullptr;
	}
	
	m_TextureMap.insert(make_pair(fileName, newTex));

	return newTex;
}

shared_ptr<Shader> GraphicResourceManager::LoadShader(const char* fileName)
{
	std::string temp = shaderPath + fileName + ".cso";
	std::wstring wpath;
	wpath.assign(temp.begin(), temp.end());
	
	shared_ptr<Shader> shader = shared_ptr<Shader>(new Shader(fileName));

	if (!shader->BuildShaders(wpath.c_str()) || !shader->CreateShader())
	{
		return nullptr;
	}

	m_ShaderMap.insert(make_pair(fileName, shader));

	return shader;
}

void GraphicResourceManager::LoadASE(const std::string& path/*, IPrefabInfo& prefabData*/)
{
	if (aseParser == nullptr)
	{
		aseParser = new ASEParse::ASEParser;
	}

	std::string filePath = "Resource\\ASE\\" + path + ".ase";

	LPSTR s = const_cast<char*>(filePath.c_str());

	//ASEParse::ASEPrefabInfo& ase = reinterpret_cast<ASEParse::ASEPrefabInfo&>(prefabData);
	
	if (!aseParser->Load(s))
	{
		return;
	}
}

void GraphicResourceManager::ReleaseResource(eResourceType type, std::string key)
{
	switch (type)
	{
		case eResourceType::Mesh:
		{
			m_MeshMap.erase(key);
			break;
		}
		case eResourceType::Material:
		{
			m_MaterialMap.erase(key);
			break;
		}
		case eResourceType::Texture:
		{
			m_TextureMap.erase(key);
			break;
		}
		case eResourceType::Shader:
		{
			m_ShaderMap.erase(key);
			break;
		}
		default:
		{
#ifdef _DEBUG
			OutputDebugString(_T("Load unexpected type file.\n"));
			assert(false);
#endif // _DEBUG
			break;
		}
	}
}

std::shared_ptr<TLGraphicsEngine::Shader> GraphicResourceManager::CreateShader(const char* name, UID id)
{
	shared_ptr<Shader> _newShader = LoadShader(name);

	if (_newShader == nullptr)
	{
		assert(false);
		return nullptr;
	}

	_newShader->SetUID(id);
	m_ShaderMap.insert(make_pair(name, _newShader));
	m_ShaderUIDMap.insert(make_pair(id, name));

	return _newShader;
}

std::shared_ptr<TLGraphicsEngine::Texture> GraphicResourceManager::CreateTexture(const char* name, UID id)
{
	shared_ptr<Texture> _newTexture = LoadTexture(name);

	if (_newTexture == nullptr)
	{
		assert(false);
		return nullptr;
	}

	_newTexture->SetUID(id);
	m_TextureMap.insert(make_pair(name, _newTexture));
	m_TextureUIDMap.insert(make_pair(id, name));

	return _newTexture;
}

std::shared_ptr<TLGraphicsEngine::Material> GraphicResourceManager::CreateMaterial(const char* name, UID id)
{
	auto _findMat = m_MaterialMap.find(name);

	if (_findMat != m_MaterialMap.end())
	{
		return _findMat->second;
	}

	shared_ptr<Material> _newMaterial = std::make_shared<Material>(name);// LoadMaterial(name);

	_newMaterial->SetUID(id);
	m_MaterialMap.insert(make_pair(name, _newMaterial));
	m_MeshUIDMap.insert(make_pair(id, name));

	return _newMaterial;
}

std::shared_ptr<TLGraphicsEngine::Mesh> GraphicResourceManager::CreateMesh(const char* name, UID id)
{

	shared_ptr<Mesh> _newMesh = GetMeshRef(name);

	if(_newMesh == nullptr)
	{
		// ase 파일이 없거나 매쉬 생성에 실패함
		assert(false);
		return nullptr;
	}

	_newMesh->SetUID(id);
	//m_MeshMap.insert(make_pair(name, _newMesh));
	m_MeshUIDMap.insert(make_pair(id, name));

	return _newMesh;
}
