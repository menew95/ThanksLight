// UnityExporter.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <stack>
#include <sstream>
#include <unordered_map>
#include <cassert>

#include "YAMLToJsonParser.h"

namespace UnityExporter
{
	namespace fs = std::filesystem;
	std::vector<std::wstring> classesName;
	std::vector<fs::path> scenesName;

	const fs::path solutionDirectory = "../";
	const fs::path engineDirectory = solutionDirectory / "GameEngine";
	const fs::path clientDirectory = solutionDirectory / "ThanksLight";
	const fs::path unityClassDirectory = solutionDirectory / "UnityClass";
	const fs::path unityClassRegistFile = "UnityClasses.h";
	const fs::path engineClassResigter = clientDirectory / "ClassResigter.h";
	const fs::path textureDirectory = solutionDirectory / "../ThanksLight_Unity/Assets/Textures";
	const fs::path unityDirectory = "../../ThanksLight_Unity";

	void RegistClassName()
	{
		if (!fs::exists(unityClassDirectory))
		{
			fs::create_directory(unityClassDirectory);
		}
		
		std::wstring unityBuiltInClass[] = {
			L"BoxCollider2D",
			L"Camera",
			L"CircleCollider2D",
			L"Collider",
			L"ColorF",
			L"EdgeCollider2D",
			L"EllipseRenderer",
			L"FileID",
			L"GameObject", 
			L"LineRenderer", 
			L"PolygonCollider2D",
			L"Rect",
			L"RectTransform",
			L"RectangleRenderer",
			L"Sprite",
			L"SpriteRenderer",
			L"SpriteSheet",
			L"Text",
			L"Texture",
			L"Transform", 
			L"Vector2",
			L"Vector3",
			L"Vector4",
		};

		std::wstringstream ss;
		ss << "#pragma once\n#include \"ClassExporter.h\"\n" << std::endl;;

		for (auto& name : classesName)
		{
			bool isBuiltInClass = false;
			for (auto builtInClassName : unityBuiltInClass)
			{
				if (name == builtInClassName)
				{
					isBuiltInClass = true;
					break;
				}
			}
			if (isBuiltInClass) continue;


			ss << "#include \"" << name << ".h\"\n";
			ss << "#include \"" << name << ".cpp\"\n";
		}
		ss << std::endl;

		ss << "namespace UnityExporter {\nvoid CreateCShapFile() {\n";
		for (auto name : classesName)
		{
			bool isBuiltInClass = false;
			for (auto builtInClassName : unityBuiltInClass)
			{
				if (name == builtInClassName)
					isBuiltInClass = true;
			}
			if (isBuiltInClass) continue;

			ss << "\tCreateClass<" << name << ">();" << std::endl;
		}
		ss << "}}" << std::endl;

		std::wfstream fs = std::wfstream(unityClassRegistFile, std::fstream::out);
		if (fs.is_open())
		{
			fs << ss.rdbuf();
			fs.close();
		}
	}

	void FindHeaderFile(const fs::path& path)
	{
		if (fs::exists(path))
		{
			for (auto& file : fs::directory_iterator(path))
			{
				const wchar_t* extension = L".h";
				if (file.path().extension() == L".h")
				{
					if (file.path().filename().string().find("Define") !=std::string::npos
						|| file.path().filename().string().find("Macro") != std::string::npos
						|| file.path().filename().string().find("MaterialData") != std::string::npos)
					{
						continue;
					}
					std::fstream fs = std::fstream(file.path(), std::fstream::in);

					std::string line;
					while (!fs.eof())
					{
						getline(fs, line);

						if (line.find("template") != std::string::npos)
						{
							getline(fs, line);
							continue;
						}

						if (!line.empty() && line.back() != ';' && line.front() != '#' && line.front() != '/' && (
							line.find(" abstract ") != std::string::npos
							))
						{
							break;
						}

						size_t pos = line.find("BOOST_DESCRIBE_");
						if (pos != std::string::npos)
						{
							classesName.push_back(file.path().filename());
							classesName.back().resize(classesName.back().length() - 2);
							break;
						}
					}
				}
			}
		}
	}

	void FindHeaderFile()
	{
		FindHeaderFile(engineDirectory);
		FindHeaderFile(clientDirectory);
	}

	//메타 파일 읽어서 GUID 가져옴
	void FindGUIDFromMetaFile(const fs::path& path, std::unordered_map<std::string, fs::path>& UIDAndFileName, fs::path extension = "")
	{
		if (fs::exists(path))
		{
			for (auto& file : fs::recursive_directory_iterator(path))
			{
				std::string fileName = file.path().filename().string();
				fs::path fileExtension = file.path().filename();
				fileExtension.replace_extension("");	//.mat.meta 에서 .meta를 날려줌

				if ((extension != "" &&  fileExtension.extension() == extension) 
					|| (extension == "" && fileName.find(".meta") != std::string::npos
					&& fileExtension.has_extension()))
				{
					std::fstream fs = std::fstream(file.path(), std::fstream::in);

					std::string line;
					while (!fs.eof())
					{
						getline(fs, line);
						if (line.find("guid") != std::string::npos)
						{
							UIDAndFileName.insert(std::make_pair(line.substr(line.find_first_of(' ') + 1), file.path().relative_path().string()));
							break;
						}
					}
				}
			}
		}
	}

	void CreateRegistClassFile()
	{
		std::wfstream fs = std::wfstream(engineClassResigter, std::wfstream::out);

		if (fs.is_open())
		{
			fs << "#pragma once\n#include \"Factory.h\"\n\n";

			for (auto& name : classesName)
			{
				fs << "#include \"" << name << ".h\"\n";
			}
			fs << std::endl;

			fs << "namespace ClassFactory {\n\
using namespace TLGameEngine;\n\
void RegistClasses() {\n";
			for (auto name : classesName)
			{
				fs << "\tRegistFactoryFromJson" << "(" << name<< "); " << std::endl;
			}
			fs << "}}" << std::endl;

			fs.close();
		}
	}

	void LoadScenesName(fs::path filePath)
	{
		std::fstream fs = std::fstream(filePath);
		std::string line;

		if (fs.is_open())
		{
			while (getline(fs, line))
			{
				if (line.find("path:") != std::string::npos)
				{
					std::string fileName = line.substr(line.find_last_of('/') + 1);

					scenesName.push_back(fileName);
				}
			}

			fs.close();
		}
	}

	void SaveScenesName(fs::path path)
	{
		if (!fs::exists(path.parent_path()))
		{
			fs::create_directory(path.parent_path());
		}
		std::fstream fs = std::fstream(path, std::fstream::out);
		
		if (fs.is_open())
		{
			for (auto sceneName : scenesName)
			{
				fs << sceneName.string().substr(0, sceneName.string().length() - sceneName.extension().string().length()) << ".json\n";
			}

			fs.close();
		}
	}

	void SaveName(fs::path fileName, std::unordered_map<std::string, std::filesystem::path>& UIDAndFileName)
	{
		std::fstream fs = std::fstream(fileName, std::fstream::out);

		if (fs.is_open())
		{
			for (auto sprite : UIDAndFileName)
			{
				fs << sprite.second.stem().string() << "\n" << sprite.first << "\n";
			}

			fs.close();
		}
	}
}

void ParseScene()
{
	std::filesystem::path unitySceneDirectory = UnityExporter::unityDirectory / "Assets/Scenes";
	std::filesystem::path outSceneDirectory = "Scenes";

	UnityExporter::LoadScenesName(UnityExporter::unityDirectory / "ProjectSettings/EditorBuildSettings.asset");
	UnityExporter::SaveScenesName(outSceneDirectory / "SceneLoadData.dat");

	for (auto sceneName : UnityExporter::scenesName)
	{
		YAMLToJsonParser::ParseToJson(unitySceneDirectory / sceneName, unitySceneDirectory, outSceneDirectory);
	}
}

void ParseMaterial()
{
	std::filesystem::path sourceDirectory = UnityExporter::unityDirectory / "Assets";
	std::filesystem::path outDir = "Materials";
	std::unordered_map<std::string, std::filesystem::path> UID_FileName;

	UnityExporter::FindGUIDFromMetaFile(sourceDirectory, UID_FileName, ".mat");
	for (auto UID_File : UID_FileName)
	{
		std::filesystem::path path = UID_File.second;
		path.replace_extension();
		YAMLToJsonParser::ParseToJson(path, sourceDirectory, outDir);
	}
	UnityExporter::SaveName(outDir / "MaterialsData.dat", UID_FileName);
}

void SaveMeshData()
{
	std::filesystem::path sourceDirectory = UnityExporter::unityDirectory / "Assets";
	std::unordered_map<std::string, std::filesystem::path> UID_FileName;

	UnityExporter::FindGUIDFromMetaFile(sourceDirectory, UID_FileName, ".fbx");

	for (auto& pair : UID_FileName)
	{
		pair.second.replace_extension();
	}

	UnityExporter::SaveName("MeshData.dat", UID_FileName);
}

void SaveShaderData()
{
	std::filesystem::path sourceDirectory = UnityExporter::unityDirectory / "Assets";
	std::unordered_map<std::string, std::filesystem::path> UID_FileName;

	UnityExporter::FindGUIDFromMetaFile(sourceDirectory, UID_FileName, ".shader");

	for (auto& pair : UID_FileName)
	{
		pair.second.replace_extension();
	}

	UnityExporter::SaveName("ShaderData.dat", UID_FileName);
}

void SaveAudioData()
{
	std::filesystem::path sourceDirectory = UnityExporter::unityDirectory / "Assets";
	std::unordered_map<std::string, std::filesystem::path> UID_FileName;

	UnityExporter::FindGUIDFromMetaFile(sourceDirectory, UID_FileName, ".mp3");
	UnityExporter::FindGUIDFromMetaFile(sourceDirectory, UID_FileName, ".wav");

	for (auto& pair : UID_FileName)
	{
		pair.second.replace_extension();
	}

	std::fstream fs = std::fstream("AudioData.dat", std::fstream::out);

	if (fs.is_open())
	{
		for (auto sprite : UID_FileName)
		{
			fs << sprite.second.string() << "\n" << sprite.first << "\n";
		}

		fs.close();
	}
}


void ParseTexture()
{
	std::filesystem::path unityTextureDirectory = UnityExporter::unityDirectory / "Assets";
	std::filesystem::path outTextureDirectory = "Textures";
	std::unordered_map<std::string, std::filesystem::path> textureUIDAndFileName;

	UnityExporter::FindGUIDFromMetaFile(unityTextureDirectory, textureUIDAndFileName, ".png");
	for (auto UID_FIle : textureUIDAndFileName)
	{
		YAMLToJsonParser::ParseToJson(UID_FIle.second, unityTextureDirectory, outTextureDirectory);
	}
	UnityExporter::SaveName(outTextureDirectory / "TexturesData.dat", textureUIDAndFileName);
}

void ParsePrefab()
{
	std::filesystem::path unityPrefabDirectory = UnityExporter::unityDirectory / "Assets";
	std::filesystem::path outPrefabDirectory = "Prefabs";
	std::unordered_map<std::string, std::filesystem::path> prefabUIDAndMataFileName;

	UnityExporter::FindGUIDFromMetaFile(unityPrefabDirectory, prefabUIDAndMataFileName, ".prefab");
	for (auto UID_File : prefabUIDAndMataFileName)
	{
		std::filesystem::path path = UID_File.second;
		path.replace_extension();
		YAMLToJsonParser::ParseToJson(path, unityPrefabDirectory, outPrefabDirectory);
	}
	UnityExporter::SaveName(outPrefabDirectory / "PrefabsData.dat", prefabUIDAndMataFileName);
}

void ParseAnimation()
{
	std::filesystem::path unityAnimationDirectory = UnityExporter::unityDirectory / "Assets/Animations";
	std::filesystem::path outAnimationDirectory = "Animations";
	std::unordered_map<std::string, std::filesystem::path> AnimationUIDAndMataFileName;

	UnityExporter::FindGUIDFromMetaFile(unityAnimationDirectory, AnimationUIDAndMataFileName, ".anim");
	for (auto& UID_File : AnimationUIDAndMataFileName)
	{
		YAMLToJsonParser::ParseToJson(UID_File.second.stem(), unityAnimationDirectory, outAnimationDirectory);
	}
	UnityExporter::SaveName(outAnimationDirectory / "AnimationClipData.dat", AnimationUIDAndMataFileName);
	
	AnimationUIDAndMataFileName.clear();
	UnityExporter::FindGUIDFromMetaFile(unityAnimationDirectory, AnimationUIDAndMataFileName, ".controller");
	for (auto& UID_File : AnimationUIDAndMataFileName)
	{
		YAMLToJsonParser::ParseToJson(UID_File.second.stem(), unityAnimationDirectory, outAnimationDirectory);
	}
	UnityExporter::SaveName(outAnimationDirectory / "AnimatorControllerData.dat", AnimationUIDAndMataFileName);
}

void ParseScript()
{
	std::filesystem::path unityScriptsDirectory = UnityExporter::unityDirectory / "Assets/Scripts";
	std::filesystem::path outScriptsDirectory = "Scripts";
	std::unordered_map<std::string, std::filesystem::path> scriptUIDAndMataFileName;

	UnityExporter::FindGUIDFromMetaFile(unityScriptsDirectory, scriptUIDAndMataFileName, ".cs");
	for (auto& UID_File : scriptUIDAndMataFileName)
	{
		UID_File.second.replace_extension("");
	}

	scriptUIDAndMataFileName.insert(std::make_pair("4e29b1a8efbd4b44bb3f3716e73f07ff", "Button"));
	scriptUIDAndMataFileName.insert(std::make_pair("5f7201a12d95ffc409449d95f23cf332", "Text"));
	scriptUIDAndMataFileName.insert(std::make_pair("fe87c0e1cc204ed48ad3b37840f39efc", "Image"));

	UnityExporter::SaveName("ScriptData.dat", scriptUIDAndMataFileName);
}

int main()
{
	UnityExporter::FindHeaderFile();
	UnityExporter::RegistClassName();
	UnityExporter::CreateRegistClassFile();

	ParseTexture();
	ParsePrefab();
	ParseAnimation();
	ParseScene();
	ParseMaterial();
	SaveMeshData();
	SaveShaderData();
	SaveAudioData();
	ParseScript();
}
