#pragma once

#include <windows.h>
#include <map>

#include "Parser.h"

#include "ASEFile.h"
#include "ParsingData.h"

using namespace std;

namespace TLGraphicsEngine
{
	class Mesh;
	class Material;
}

namespace ASEParse
{
	class ASEParser :
		public Parser
	{
	public:
		ASEParser();
		virtual ~ASEParser();

	private:
		ASEParse::ASELexer* m_lexer = nullptr;

	private:
		SceneInfo m_CurrSceneInfo;
		/// 한 씬에 N개의 메시가 존재할 수 있다.
		TLGraphicsEngine::Mesh* m_LoadMesh;						// 메시 하나의 데이터가 들어갈 구조체

		bool m_HasNegativeScale = false;
		bool m_HasNegativeScaleX = false;
		bool m_HasNegativeScaleY = false;
		bool m_HasNegativeScaleZ = false;

		std::string m_currFilePath;

		MeshInfo meshInfo;

		std::vector<std::string> m_MaterialIDs;

		std::vector<vector<int>> m_SameVertexIndexLists;

	private:
		char m_TokenString[256] = {0};
		int m_Token = 0;
		int m_parsingmode = 0;
		bool m_ChangeYZ = false;
	public:
		// 초기화
		virtual bool Initialize();

		/// <summary>
		/// Mesh는 그래픽스 엔진 리소스 매니저에 등록
		/// </summary>
		/// <param name="filePath">파일 위치</param>
		/// <param name="prefab">GameEngine Prefab Ref</param>
		/// <returns>파싱 성공 여부</returns>
		bool Load(const LPSTR filePath);

		/// <summary>
		/// TM를 로컬 기준으로 재계산
		/// </summary>
		/// <returns></returns>
		bool SettingAllNodeTMToLocalTM(ASEPrefabInfo& asePrefab);

		//GraphicsEngine::GameObject* MakeGameObject(const LPSTR filepath);
	private:
		bool CalcLocalTM(ASEPrefabInfo& asePrefab, ASEObject& aseObj, ASEObject& parentObj);

	private:
		int Parsing_NumberLong();			// long 를 읽어서 리턴해준다.
		float Parsing_NumberFloat();		// float
		LPSTR Parsing_String();				// string
		int Parsing_NumberInt();			// int
		Vector3 Parsing_NumberVector3();	// 3개의 Float 를 벡터 하나로 읽어서 리턴해준다.

	private:
		void ParsingASE(ASEPrefabInfo& prefab);

		void ParsingScene();

		void ParsingObject(ASEPrefabInfo& asePrefab);

		void ParsingNodeTM(ASEObject& aseObject);

		void ParsingMesh(ASEObject& aseObject);
		void ParsingBoundingBox();
		void ParsingMesh_Vertices(ASEObject& aseObject, vector<VertexInfo>& vertices);
		void ParsingMesh_Facies(vector<FaceInfo>& facies);
		void ParsingMesh_Normals(ASEObject& aseObject, vector<VertexInfo>& vertices, vector<FaceInfo>& facies);

		void ParsingMesh_TextureVerteies(vector<VertexInfo>& vertices, vector<FaceInfo>& facies, vector<Vector2>& uvs);
		void ParsingMesh_UVtoVertex(vector<VertexInfo>& vertices, vector<FaceInfo>& facies, vector<Vector2>& uvs);

		void ParsingAnimation(ASEPrefabInfo& asePrefab, ASEObject& aseObject);
		void ParsingAnimationPos(AnimationInfo& animInfo);
		void ParsingAnimationRot(AnimationInfo& animInfo);
		void ParsingAnimationScale(AnimationInfo& animInfo);

		void ParsingBoneList(ASEPrefabInfo& asePrefab);
		void ParsingVertexWeightList(ASEPrefabInfo& asePrefab);
		void Parsing_Weight(int vertexIdx);
		void ParsingMarterialList();
		void ParsingMarterial();
		void ParsingMapBump(TLGraphicsEngine::Material* mat);
		void ParsingMapDiffuse(TLGraphicsEngine::Material* mat);
		void ParsingMapMR(TLGraphicsEngine::Material* mat);
		void ParsingMapGeneric(TLGraphicsEngine::Material* mat);
	};


};