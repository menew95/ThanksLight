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
		/// �� ���� N���� �޽ð� ������ �� �ִ�.
		TLGraphicsEngine::Mesh* m_LoadMesh;						// �޽� �ϳ��� �����Ͱ� �� ����ü

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
		// �ʱ�ȭ
		virtual bool Initialize();

		/// <summary>
		/// Mesh�� �׷��Ƚ� ���� ���ҽ� �Ŵ����� ���
		/// </summary>
		/// <param name="filePath">���� ��ġ</param>
		/// <param name="prefab">GameEngine Prefab Ref</param>
		/// <returns>�Ľ� ���� ����</returns>
		bool Load(const LPSTR filePath);

		/// <summary>
		/// TM�� ���� �������� ����
		/// </summary>
		/// <returns></returns>
		bool SettingAllNodeTMToLocalTM(ASEPrefabInfo& asePrefab);

		//GraphicsEngine::GameObject* MakeGameObject(const LPSTR filepath);
	private:
		bool CalcLocalTM(ASEPrefabInfo& asePrefab, ASEObject& aseObj, ASEObject& parentObj);

	private:
		int Parsing_NumberLong();			// long �� �о �������ش�.
		float Parsing_NumberFloat();		// float
		LPSTR Parsing_String();				// string
		int Parsing_NumberInt();			// int
		Vector3 Parsing_NumberVector3();	// 3���� Float �� ���� �ϳ��� �о �������ش�.

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