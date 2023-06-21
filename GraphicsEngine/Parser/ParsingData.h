#pragma once

#include <vector>
#include <map>
#include <string>

#include "SimpleMath.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

struct IPrefabInfo
{
	enum class ePrefabType
	{
		ASE = 0,
		FBX = 1,
		UnKnown = 255
	};

	IPrefabInfo(ePrefabType type) : m_PrefabType(type) {};

	ePrefabType m_PrefabType = ePrefabType::UnKnown;
};

namespace ASEParse
{
	struct PosTrack
	{
		float m_FrameRate = 0;
		Vector3 m_Pos_Sample;

		PosTrack(float frameRate, Vector3 pos);
	};

	struct RotTrack
	{
		float m_FrameRate = 0;
		Quaternion m_Rot_Sample;
		Vector3 m_eulerRot_Sample;

		RotTrack(float frameRate, Quaternion rot);
		RotTrack(float frameRate, Quaternion rot, Vector3 eulerRot);
	};

	struct ScaleTrack
	{
		float m_FrameRate = 0;
		Vector3 m_Scale_Sample;

		ScaleTrack(float frameRate, Vector3 scale);
	};

	struct AnimationInfo
	{
		std::string m_InfoName;
		float m_MinFrameRate = 0;
		float m_MaxFrameRate = 0;
		std::vector<PosTrack> m_PosTrackList;
		std::vector<RotTrack> m_RotTrackList;
		std::vector<ScaleTrack> m_ScaleTrackList;
	};

	struct SceneInfo
	{
		std::string m_FileName;
		float m_FirstFrame		= 0;
		float m_LastFrame		= 0;
		float m_FrameSpeed		= 0;
		float m_TicksPerFrame	= 0;
		Vector3 m_Background_Static;
		Vector3 m_Ambient_Static;
	};

	struct VertexInfo
	{
		VertexInfo();

		// 좌표상의 위치값
		Vector3 m_pos;

		// 노말값
		Vector3 m_normal;

		// 노말값
		Vector3 m_Tangent;

		// 노말값이 셋팅됐는가?
		bool m_IsNormalSet;

		// 텍스쳐 좌표
		float u, v;

		// 텍스쳐값이 셋팅됐는가?
		bool m_IsTextureSet;

		/// Skinned Mesh 에서 추가
		// weight 3개
		Vector4 m_bones;
		Vector4 m_weights;

		// 인덱스
		int m_indices;

		int m_matRef;
	};

	struct MeshInfo
	{
		std::vector<VertexInfo> m_vertices;
		std::vector<std::vector<UINT>> m_subMeshs;


	};

	struct NodeTM
	{
		// 노드 이름
		std::string m_NodeName;

		bool m_IsLocalTM;

		// Pos, Rot, Scale
		Vector3 m_Inherit_Position;
		Vector3 m_Inherit_Rotate;
		Vector3 m_Inherit_Scale;

		// Transform matrix
		Matrix m_TM;
		Matrix m_LocalTM;

		// 분리된 Pos, Rot, Scale 정보들
		Vector3 m_TM_Position;
		Vector3 m_TM_Rotate;
		Vector3 m_TM_RotateAxis;
		float m_TM_RotateAngle;
		Vector3 m_TM_Scale;
		Vector3 m_TM_ScaleAxis;
		float m_TM_ScaleAxisAngle;

		NodeTM()
			: m_NodeName(), m_IsLocalTM(false)
			, m_Inherit_Position(0.f, 0.f, 0.f), m_Inherit_Rotate(0.f, 0.f, 0.f), m_Inherit_Scale(0.f, 0.f, 0.f)
			, m_TM_Position(0.f, 0.f, 0.f)
			, m_TM_Rotate(0.f, 0.f, 0.f), m_TM_RotateAxis(0.f, 0.f, 0.f), m_TM_RotateAngle(0.f)
			, m_TM_Scale(0.f, 0.f, 0.f), m_TM_ScaleAxis(0.f, 0.f, 0.f), m_TM_ScaleAxisAngle(0.f)
		{
			m_TM =
			{
				0.f,0.f,0.f,0.f,
				0.f,0.f,0.f,0.f,
				0.f,0.f,0.f,0.f,
				0.f,0.f,0.f,1.f
			};
		}
	};

	struct FaceInfo
	{
		// 이 Face 를 이루는 버텍스의 인덱스
		int				m_vertexindex[3];
		// Face 의 Normal 값
		Vector3			m_normal;

		// Vertex 의 Normal 값
		Vector3			m_normalvertex[3];
		// Texture Coordinate
		Vector3			m_TFace;

		int m_materialRef;

		// 초기화를 위한 생성자.
		FaceInfo();
	};

	/// <summary>
	/// Bone 구조체
	/// </summary>
	struct Bone
	{
		int m_Index = 0;
		std::string m_Bone_Name;

		Matrix m_BoneTM_NodeTM;
		Matrix m_BoneTM_WorldTM;
	};

	/// <summary>
	/// Bone 과 연결 될 가중치
	/// </summary>
	struct Weight
	{
		int	m_Bone_Idx;
		float m_Bone_Weight;	
	};
	
	struct VertexWeight
	{
		int m_Vertex_Index;

		Weight* m_Bone_Blend_Weight;
		std::vector<Weight*> m_Bone_Blending_Weight;
	};

	/// <summary>
	/// 하나의 게임 오브젝트를 구성할 정보
	/// </summary>
	struct ASEObject
	{
		ASEObject() : m_ObjectName(), m_ParentObject(), m_MeshID(), m_MaterialID() {};
		// Object
		std::string m_ObjectName;

		// Node
		NodeTM m_Node;

		// Node Info
		bool m_HasParent = false;
		std::string m_ParentObject;

		// Mesh Info & Material Info
		bool m_HasMesh = false;
		bool m_IsSkined = false;
		std::string m_MeshID;
		bool m_HasMaterial = false;
		std::string m_MaterialID;

		bool m_HasAnimationInfo = false;
		AnimationInfo m_AnimationInfo;
		
		// Todo : 여러개의 애니메이션을 받도록 수정 필요
		// 아직 ASE Exporter가 안되서 정하지 못함
		//std::vector<AnimationInfo> m_AnimationInfos;

		/// 나머지 정보들
		int				m_Prop_Motionblur = 0;			//*PROP_MOTIONBLUR 0
		int				m_Prop_Castshadow = 0;			//*PROP_CASTSHADOW 1
		int				m_Prop_Recvshadow = 0;			//*PROP_RECVSHADOW 1
		int				m_Material_Ref = 0;			// 이 메쉬의 매트리얼의 번호
		Vector3			m_Wireframe_Color;
	};


	/// <summary>
	/// 게임 엔진 팩토리에 줄 프리팹 정보
	/// </summary>
	struct ASEPrefabInfo : IPrefabInfo
	{
		ASEPrefabInfo() : IPrefabInfo(ePrefabType::ASE) {};

		std::map<std::string, ASEObject> m_ASEObjects;

		// Skinned Mesh Info
		bool m_HasSkined = false;
		std::vector<Bone> m_BoneList;
		std::vector<VertexWeight> m_VertexWeightList;

		// Animation Info
		bool m_HasAnimation = false;
	};
}