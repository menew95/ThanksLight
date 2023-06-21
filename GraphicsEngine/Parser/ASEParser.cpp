#include <assert.h>

#include "ASEFile.h"
#include "ParsingData.h"
#include "ASEParser.h"
#include "../GraphicResourceManager.h"

ASEParse::ASEParser::ASEParser()
{

}

ASEParse::ASEParser::~ASEParser()
{
	if (m_lexer != nullptr)
	{
		delete m_lexer;
		m_lexer = nullptr;
	}
}

bool ASEParse::ASEParser::Initialize()
{
	m_lexer = new ASEParse::ASELexer;

	return TRUE;
}

bool ASEParse::ASEParser::Load(const LPSTR filePath)
{
	ASEPrefabInfo prefab;

	if (m_lexer == nullptr)
	{
		Initialize();
	}
	/// 0) 파일을 로드한다.
	if (!m_lexer->Open(filePath))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return false;
	}

	meshInfo.m_vertices.clear();
	meshInfo.m_subMeshs.clear();
	meshInfo.m_subMeshs.resize(1);
	ParsingASE(prefab);
	SettingAllNodeTMToLocalTM(prefab);

#ifdef _DEBUG
	for (auto& item : prefab.m_ASEObjects)
	{
		if (!item.second.m_Node.m_IsLocalTM)
		{
			std::string temp = item.second.m_ObjectName + " : Local TM 적용 이 안됨 오브젝트.\n";
			OutputDebugStringA(temp.c_str());
		}
	}
#endif // _DEBUG

	m_lexer->Close();

	return true;
}

bool ASEParse::ASEParser::SettingAllNodeTMToLocalTM(ASEPrefabInfo& asePrefab)
{
	for (auto& value : asePrefab.m_ASEObjects)
	{
		ASEObject& aseObject = value.second;

		// 부모가 없으면 이미 로컬 기준
		if (!aseObject.m_HasParent)
		{
			aseObject.m_Node.m_IsLocalTM = true;
			aseObject.m_Node.m_LocalTM = aseObject.m_Node.m_TM;
			continue;
		}

		auto parentIter = asePrefab.m_ASEObjects.find(aseObject.m_ParentObject);

		if (parentIter == asePrefab.m_ASEObjects.end())
		{
			// 부모가 있는데 맵에 키에 해당하는 아이템이 없음
			return false;
		}

		ASEObject& parent = (*parentIter).second;

		if (!CalcLocalTM(asePrefab, aseObject, parent))
		{
			return false;
		}
	}

	return true;
}

bool ASEParse::ASEParser::CalcLocalTM(ASEPrefabInfo& asePrefab, ASEObject& aseObj, ASEObject& parentObj)
{
	if (parentObj.m_HasParent == false)
	{
		// 부모의 부모가 없을 경우 이미 로컬TM
		parentObj.m_Node.m_IsLocalTM = true;
		parentObj.m_Node.m_LocalTM = parentObj.m_Node.m_TM;
	}
	else
	{
		if (!parentObj.m_Node.m_IsLocalTM)
		{
			// 부모 노드TM이 아직 로컬  안됨
			auto parentIter = asePrefab.m_ASEObjects.find(parentObj.m_ParentObject);

			if (parentIter == asePrefab.m_ASEObjects.end())
			{
				// 부모가 있는데 맵에 키에 해당하는 아이템이 없음
				OutputDebugStringA("부모가 있는데 맵에 키에 해당하는 아이템이 없음");
				return false;
			}

			CalcLocalTM(asePrefab, parentObj, (*parentIter).second);
		}
	}

	aseObj.m_Node.m_LocalTM = aseObj.m_Node.m_TM * parentObj.m_Node.m_TM.Invert();

	aseObj.m_Node.m_IsLocalTM = true;

	return true;
}

int ASEParse::ASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

float ASEParse::ASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

LPSTR ASEParse::ASEParser::Parsing_String()
{
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}

int ASEParse::ASEParser::Parsing_NumberInt()
{
	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}

Vector3 ASEParse::ASEParser::Parsing_NumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	if (m_ChangeYZ)
	{
		token = m_lexer->GetToken(m_TokenString);
		tempVector3.z = (float)atof(m_TokenString);
		token = m_lexer->GetToken(m_TokenString);
		tempVector3.y = (float)atof(m_TokenString);
	}
	else
	{
		token = m_lexer->GetToken(m_TokenString);
		tempVector3.y = (float)atof(m_TokenString);
		token = m_lexer->GetToken(m_TokenString);
		tempVector3.z = (float)atof(m_TokenString);
	}

	return			tempVector3;		// 스태틱 변수의 레퍼런스보다는 값 전달을 하자.
}

void ASEParse::ASEParser::ParsingASE(ASEPrefabInfo& prefab)
{
	LONG nowtoken;

	bool end = false;
	int a = 0;
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_END)
	{
		switch (nowtoken)
		{
		case TOKENR_COMMENT:
		{
			break;
		}
		case TOKENR_SCENE:
		{
			ParsingScene();
			
			break;
		}
		case TOKENR_GEOMOBJECT:
		{
			ParsingObject(prefab);
			
			break;
		}
		case TOKENR_HELPEROBJECT:
		{
			ParsingObject(prefab);
			
			break;
		}
		case TOKENR_SHAPEOBJECT:
		{
			ParsingObject(prefab);
			
			break;
		}
		case TOKENR_MATERIAL_LIST:
		{

			ParsingMarterialList();

			
			break;
		}
		case TOKEND_END:
		{
			return;
			break;
		}
		}
	}

	
}

void ASEParse::ASEParser::ParsingScene()
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_SCENE_FILENAME:
		{
			m_ChangeYZ = true;
			m_CurrSceneInfo.m_FileName = Parsing_String();
			if (m_CurrSceneInfo.m_FileName.find(".max") != std::string::npos)
				m_ChangeYZ = true;
			break;
		}
		case TOKENR_SCENE_FIRSTFRAME:
		{
			m_CurrSceneInfo.m_FirstFrame = Parsing_NumberFloat();
			break;
		}
		case TOKENR_SCENE_LASTFRAME:
		{
			m_CurrSceneInfo.m_LastFrame = Parsing_NumberFloat();
			break;
		}
		case TOKENR_SCENE_FRAMESPEED:
		{
			m_CurrSceneInfo.m_FrameSpeed = Parsing_NumberFloat();
			break;
		}
		case TOKENR_SCENE_TICKSPERFRAME:
		{
			m_CurrSceneInfo.m_TicksPerFrame = Parsing_NumberFloat();
			break;
		}
		case TOKENR_SCENE_BACKGROUND_STATIC:
		{
			m_CurrSceneInfo.m_Background_Static = Parsing_NumberVector3();
			break;
		}
		case TOKENR_SCENE_AMBIENT_STATIC:
		{
			m_CurrSceneInfo.m_Ambient_Static = Parsing_NumberVector3();
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingObject(ASEPrefabInfo& asePrefab)
{
	LONG nowtoken;

	m_HasNegativeScale = false;
	m_HasNegativeScaleX = false;
	m_HasNegativeScaleY = false;
	m_HasNegativeScaleZ = false;

	ASEObject* aseObject = nullptr;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_NODE_NAME:
		{
			auto name = Parsing_String();
			aseObject = &asePrefab.m_ASEObjects.insert(make_pair(name, ASEObject())).first->second;
			aseObject->m_ObjectName = name;
			break;
		}
		case TOKENR_NODE_PARENT:
		{
			aseObject->m_ParentObject = Parsing_String();
			aseObject->m_HasParent = true;
			break;
		}
		case TOKENR_NODE_TM:
			ParsingNodeTM(*aseObject);
			break;
		case TOKENR_MESH:
			ParsingMesh(*aseObject);
			break;
		case TOKENR_TM_ANIMATION:
			ParsingAnimation(asePrefab, *aseObject);
			break;
		case TOKENR_PROP_MOTIONBLUR:
			aseObject->m_Prop_Motionblur = Parsing_NumberInt();
			break;
		case TOKENR_PROP_CASTSHADOW:
			aseObject->m_Prop_Castshadow = Parsing_NumberInt();
			break;
		case TOKENR_PROP_RECVSHADOW:
			aseObject->m_Prop_Recvshadow = Parsing_NumberInt();
			break;
		case TOKENR_MATERIAL_REF:
			aseObject->m_HasMaterial = true;
			aseObject->m_Material_Ref = Parsing_NumberInt();

			/*if (aseObject->m_Material_Ref < static_cast<int>(m_MaterialIDs.size()))
			{
				aseObject->m_MaterialID = m_MaterialIDs[aseObject->m_Material_Ref];
			}
			else
			{
				assert(false);
			}*/
			break;
		case TOKENR_WIREFRAME_COLOR:
			aseObject->m_Wireframe_Color = Parsing_NumberVector3();
			break;
		case TOKENR_MESH_NUMBONE:
			asePrefab.m_BoneList.resize(Parsing_NumberInt());
			asePrefab.m_HasSkined = true;
			aseObject->m_IsSkined = true;
			break;
		case TOKENR_BONE_LIST:
			ParsingBoneList(asePrefab);
			break;
		case TOKENR_MESH_WVERTEXS:
			ParsingVertexWeightList(asePrefab);
			break;
		}
	}

	if (aseObject != nullptr && m_LoadMesh)
		TLGraphicsEngine::GraphicResourceManager::Instance()->RegistMeshBuffer(aseObject->m_ObjectName, m_LoadMesh);
}

void ASEParse::ASEParser::ParsingNodeTM(ASEObject& aseObject)
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_NODE_NAME:
		{
			aseObject.m_Node.m_NodeName = Parsing_String();
			break;
		}
		case TOKENR_INHERIT_POS:
		{
			aseObject.m_Node.m_Inherit_Position = Parsing_NumberVector3();
			break;
		}
		case TOKENR_INHERIT_ROT:
		{
			aseObject.m_Node.m_Inherit_Rotate = Parsing_NumberVector3();
			break;
		}
		case TOKENR_INHERIT_SCL:
		{
			aseObject.m_Node.m_Inherit_Scale = Parsing_NumberVector3();
			break;
		}
		case TOKENR_TM_ROW0:
		{
			Vector3 row0 = Parsing_NumberVector3();

			aseObject.m_Node.m_TM.m[0][0] = row0.x;
			aseObject.m_Node.m_TM.m[0][1] = row0.y;
			aseObject.m_Node.m_TM.m[0][2] = row0.z;
			break;
		}
		case TOKENR_TM_ROW1:
		{
			Vector3 row1 = Parsing_NumberVector3();
			aseObject.m_Node.m_TM.m[1][0] = row1.x;
			aseObject.m_Node.m_TM.m[1][1] = row1.y;
			aseObject.m_Node.m_TM.m[1][2] = row1.z;
			break;
		}
		case TOKENR_TM_ROW2:
		{
			Vector3 row1, row2;
			if (m_ChangeYZ)
			{
				row1 = Parsing_NumberVector3();
				row2 = {
					aseObject.m_Node.m_TM.m[1][0],
					aseObject.m_Node.m_TM.m[1][1],
					aseObject.m_Node.m_TM.m[1][2]
				};

				aseObject.m_Node.m_TM.m[1][0] = row1.x;
				aseObject.m_Node.m_TM.m[1][1] = row1.y;
				aseObject.m_Node.m_TM.m[1][2] = row1.z;

			}
			else
			{
				row2 = Parsing_NumberVector3();
			}

			aseObject.m_Node.m_TM.m[2][0] = row2.x;
			aseObject.m_Node.m_TM.m[2][1] = row2.y;
			aseObject.m_Node.m_TM.m[2][2] = row2.z;
			break;
		}
		case TOKENR_TM_ROW3:
		{
			Vector3 row3 = Parsing_NumberVector3();
			aseObject.m_Node.m_TM.m[3][0] = row3.x;
			aseObject.m_Node.m_TM.m[3][1] = row3.y;
			aseObject.m_Node.m_TM.m[3][2] = row3.z;


			XMVECTOR _det = XMMatrixDeterminant(aseObject.m_Node.m_TM);

			// Negative Scale Object
			if (DirectX::XMVectorGetX(_det) < 0)
			{
				m_HasNegativeScale = true;

				XMVECTOR _scale;

				XMVECTOR _rotationQuat;

				XMVECTOR _translation;

				XMMatrixDecompose(&_scale, &_rotationQuat, &_translation, aseObject.m_Node.m_TM);

				_scale = -1 * _scale;

				// NodeTM을 재조립
				aseObject.m_Node.m_TM = XMMatrixScalingFromVector(_scale) * XMMatrixRotationQuaternion(_rotationQuat)
					* XMMatrixTranslationFromVector(_translation);

			}
			break;
		}
		case TOKENR_TM_POS:
		{
			aseObject.m_Node.m_TM_Position = Parsing_NumberVector3();
			break;
		}
		case TOKENR_TM_ROTAXIS:
		{
			aseObject.m_Node.m_TM_RotateAxis = Parsing_NumberVector3();
			break;
		}
		case TOKENR_TM_ROTANGLE:
		{
			aseObject.m_Node.m_TM_RotateAngle = Parsing_NumberFloat();
			break;
		}
		case TOKENR_TM_SCALE:
		{
			aseObject.m_Node.m_TM_Scale = Parsing_NumberVector3();
			break;
		}
		case TOKENR_TM_SCALEAXIS:
		{
			aseObject.m_Node.m_TM_ScaleAxis = Parsing_NumberVector3();
			break;
		}
		case TOKENR_TM_SCALEAXISANG:
		{
			aseObject.m_Node.m_TM_ScaleAxisAngle = Parsing_NumberFloat();
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingMesh(ASEObject& aseObject)
{
	LONG nowtoken;

	TLGraphicsEngine::Mesh* newMesh = new TLGraphicsEngine::Mesh(aseObject.m_ObjectName.c_str());
	m_LoadMesh = newMesh;
	aseObject.m_MeshID = aseObject.m_ObjectName;

	std::vector<FaceInfo> facies;
	std::vector<Vector2> UVs;

	m_SameVertexIndexLists.clear();

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_TIMEVALUE:
			break;
		case TOKENR_MESH_NUMVERTEX:
			m_SameVertexIndexLists.resize(Parsing_NumberInt());
			break;
		case TOKENR_MESH_NUMFACES:
			break;
		case TOKENR_MESH_VERTEX_LIST:
			ParsingMesh_Vertices(aseObject, meshInfo.m_vertices);
			break;
		case TOKENR_MESH_FACE_LIST:
			ParsingMesh_Facies(facies);
			break;
		case TOKENR_MESH_TVERTLIST: //UV 좌표들
		{
			ParsingMesh_TextureVerteies(meshInfo.m_vertices, facies, UVs);
			break;
		}
		case TOKENR_MESH_TFACELIST: // UV 좌표가 들어갈 버텍스
		{
			ParsingMesh_UVtoVertex(meshInfo.m_vertices, facies, UVs);
			break;
		}
		case TOKENR_MESH_NORMALS:
		{
			ParsingMesh_Normals(aseObject, meshInfo.m_vertices, facies);
			break;
		}
		case TOKENR_MESH_CVERTLIST:
		{
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
			{
			}
			int a = 0;
			break;
		}
		case TOKENR_MESH_CFACELIST:
		{
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
			{
			}
			int a = 0;
			break;
		}
		case TOKENR_MESH_MAPPINGCHANNEL:
		{
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
			{
				switch (nowtoken)
				{
				case TOKENR_MESH_TVERTLIST:
				{
					while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
					{
					}
					break;
				}
				case TOKENR_MESH_TFACELIST:
				{
					while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
					{
					}
					break;
				}
				}
			}
			int a = 0;
			break;
		}
		}
	}


	auto _vertexBuffer = std::make_shared<TLGraphicsEngine::IndexBuffer>();
	auto _indexBuffer = std::make_shared<TLGraphicsEngine::VertexBuffer>();


	std::vector<TLGraphicsEngine::Vertex> _tempVertices;
	for (auto& vertex : meshInfo.m_vertices)
	{
		TLGraphicsEngine::Vertex copyV;
		copyV.Pos = vertex.m_pos;
		copyV.Normal = vertex.m_normal;
		copyV.UV = { vertex.u, vertex.v };
		copyV.Tangent = vertex.m_Tangent;
		_tempVertices.push_back(copyV);
	}

	m_LoadMesh->SetVertices(_tempVertices);

	for (auto& indices : meshInfo.m_subMeshs)
	{
		m_LoadMesh->SetTriangles(indices);
	}

	ParsingBoundingBox();
	aseObject.m_HasMesh = true;
}

void ASEParse::ASEParser::ParsingBoundingBox()
{
	if (m_LoadMesh->m_MinBox.x == 0 && m_LoadMesh->m_MinBox.y == 0 && m_LoadMesh->m_MinBox.z == 0 &&
		m_LoadMesh->m_MaxBox.x == 0 && m_LoadMesh->m_MaxBox.y == 0 && m_LoadMesh->m_MaxBox.z == 0)
	{
		return;
	}

	m_LoadMesh->m_BoundingBoxVertices.push_back(TLGraphicsEngine::Vertex(DirectX::XMFLOAT3(m_LoadMesh->m_MinBox.x, m_LoadMesh->m_MinBox.y, m_LoadMesh->m_MinBox.z)));
	m_LoadMesh->m_BoundingBoxVertices.push_back(TLGraphicsEngine::Vertex(DirectX::XMFLOAT3(m_LoadMesh->m_MinBox.x, m_LoadMesh->m_MaxBox.y, m_LoadMesh->m_MinBox.z)));
	m_LoadMesh->m_BoundingBoxVertices.push_back(TLGraphicsEngine::Vertex(DirectX::XMFLOAT3(m_LoadMesh->m_MaxBox.x, m_LoadMesh->m_MaxBox.y, m_LoadMesh->m_MinBox.z)));
	m_LoadMesh->m_BoundingBoxVertices.push_back(TLGraphicsEngine::Vertex(DirectX::XMFLOAT3(m_LoadMesh->m_MaxBox.x, m_LoadMesh->m_MinBox.y, m_LoadMesh->m_MinBox.z)));
	m_LoadMesh->m_BoundingBoxVertices.push_back(TLGraphicsEngine::Vertex(DirectX::XMFLOAT3(m_LoadMesh->m_MinBox.x, m_LoadMesh->m_MinBox.y, m_LoadMesh->m_MaxBox.z)));
	m_LoadMesh->m_BoundingBoxVertices.push_back(TLGraphicsEngine::Vertex(DirectX::XMFLOAT3(m_LoadMesh->m_MinBox.x, m_LoadMesh->m_MaxBox.y, m_LoadMesh->m_MaxBox.z)));
	m_LoadMesh->m_BoundingBoxVertices.push_back(TLGraphicsEngine::Vertex(DirectX::XMFLOAT3(m_LoadMesh->m_MaxBox.x, m_LoadMesh->m_MaxBox.y, m_LoadMesh->m_MaxBox.z)));
	m_LoadMesh->m_BoundingBoxVertices.push_back(TLGraphicsEngine::Vertex(DirectX::XMFLOAT3(m_LoadMesh->m_MaxBox.x, m_LoadMesh->m_MinBox.y, m_LoadMesh->m_MaxBox.z)));

	// Index
	UINT indTemp[] =
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	for (auto index : indTemp)
	{
		m_LoadMesh->m_BoundingBoxTriangles.push_back(index);
	}
}

void ASEParse::ASEParser::ParsingMesh_Vertices(ASEObject& aseObject, vector<VertexInfo>& vertices)
{
	LONG nowtoken;

	auto inverTM = aseObject.m_Node.m_TM.Invert();

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MESH_VERTEX:
		{
			VertexInfo vertex;
			int i = Parsing_NumberInt();
			vertex.m_pos = Parsing_NumberVector3();
			vertex.m_pos = Vector3::Transform(vertex.m_pos, inverTM);
			vertices.push_back(vertex);

			if (m_LoadMesh->m_MinBox.x > vertex.m_pos.x)
			{
				m_LoadMesh->m_MinBox.x = vertex.m_pos.x;
			}
			if (m_LoadMesh->m_MinBox.y > vertex.m_pos.y)
			{
				m_LoadMesh->m_MinBox.y = vertex.m_pos.y;
			}
			if (m_LoadMesh->m_MinBox.z > vertex.m_pos.z)
			{
				m_LoadMesh->m_MinBox.z = vertex.m_pos.z;
			}
			if (m_LoadMesh->m_MaxBox.x < vertex.m_pos.x)
			{
				m_LoadMesh->m_MaxBox.x = vertex.m_pos.x;
			}
			if (m_LoadMesh->m_MaxBox.y < vertex.m_pos.y)
			{
				m_LoadMesh->m_MaxBox.y = vertex.m_pos.y;
			}
			if (m_LoadMesh->m_MaxBox.z < vertex.m_pos.z)
			{
				m_LoadMesh->m_MaxBox.z = vertex.m_pos.z;
			}
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingMesh_Facies(vector<FaceInfo>& facies)
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MESH_FACE:
		{
			FaceInfo face;

			m_lexer->GetToken(m_TokenString); // id:

			m_lexer->GetToken(m_TokenString); // A:
			face.m_vertexindex[0] = Parsing_NumberInt();
			m_lexer->GetToken(m_TokenString); // B:
			face.m_vertexindex[1] = Parsing_NumberInt();
			m_lexer->GetToken(m_TokenString); // C:
			face.m_vertexindex[2] = Parsing_NumberInt();


			m_lexer->GetToken(m_TokenString); // AB:
			Parsing_NumberInt();
			m_lexer->GetToken(m_TokenString); // BC:
			Parsing_NumberInt();
			m_lexer->GetToken(m_TokenString); // CA:
			Parsing_NumberInt();

			m_lexer->GetToken(m_TokenString); // MESH_SMOOTHING:
			Parsing_NumberInt();
			m_lexer->GetToken(m_TokenString); // MESH_MTLID:
			face.m_materialRef = Parsing_NumberInt();

			facies.push_back(face);
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingMesh_Normals(ASEObject& aseObject, vector<VertexInfo>& vertices, vector<FaceInfo>& facies)
{
	LONG nowtoken;

	auto invertTM = aseObject.m_Node.m_TM.Invert();

	//m_SameVertexIndexLists.resize(vertices.size());

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MESH_FACENORMAL:
		{
			int faceID = Parsing_NumberInt();
			int vertexID[3] = { 0, 0, 0 };
			int newVertexID = 0;
			facies[faceID].m_normal = Parsing_NumberVector3();

			int index[3] = { 0, 0, 0 }, tempIdx = 0;

			for (int i = 0; i < 3; i++)
			{
				// *MESH_VERTEXNORMAL 1
				if (m_lexer->GetToken(m_TokenString) == TOKENR_MESH_VERTEXNORMAL)
				{
					vertexID[i] = Parsing_NumberInt();
					bool isUVVertex = false;
					int uvVertex = 0;
					if (vertexID[i] != facies[faceID].m_vertexindex[i])
					{
						isUVVertex = true;
						uvVertex = facies[faceID].m_vertexindex[i];
					}
					Vector3 normal = Parsing_NumberVector3();

					//normal = Vector3::TransformNormal(normal, invertTM);
					if (m_HasNegativeScale)
					{
						normal *= -1;
					}

					if (vertices[isUVVertex ? uvVertex : vertexID[i]].m_IsNormalSet)
					{
						bool isNewNormal = true;
						for (auto id : m_SameVertexIndexLists[vertexID[i]])
						{
							if (normal == vertices[id].m_normal)
							{
								isNewNormal = false;
								tempIdx = id;
								break;
							}
						}
						if (isNewNormal)
						{
							VertexInfo cVertex(vertices[vertexID[i]]);
							cVertex.m_normal = normal;
							vertices.push_back(cVertex);
							m_SameVertexIndexLists[vertexID[i]].push_back((int)vertices.size() - 1);
							index[i] = (int)vertices.size() - 1;
						}
						else
						{
							index[i] = tempIdx;
						}
					}
					else
					{
						int tempIndex = isUVVertex ? uvVertex : vertexID[i];
						vertices[tempIndex].m_IsNormalSet = true;
						vertices[tempIndex].m_normal = normal;
						m_SameVertexIndexLists[vertexID[i]].push_back(tempIndex);
						index[i] = tempIndex;
					}
				}
			}

			if (m_ChangeYZ)
			{
				meshInfo.m_subMeshs[facies[faceID].m_materialRef].push_back(index[0]);
				meshInfo.m_subMeshs[facies[faceID].m_materialRef].push_back(index[2]);
				meshInfo.m_subMeshs[facies[faceID].m_materialRef].push_back(index[1]);

				// e1 = p1 - p0, e2 = p2 - p0
				XMVECTOR p0 = { vertices[index[0]].m_pos.x, vertices[index[0]].m_pos.y, vertices[index[0]].m_pos.z };
				XMVECTOR p1 = { vertices[index[2]].m_pos.x, vertices[index[2]].m_pos.y, vertices[index[2]].m_pos.z };
				XMVECTOR p2 = { vertices[index[1]].m_pos.x, vertices[index[1]].m_pos.y, vertices[index[1]].m_pos.z };

				XMVECTOR e1 = p1 - p0;
				XMVECTOR e2 = p2 - p0;

				float x1 = vertices[index[2]].u - vertices[index[0]].u; // u1 - u0
				float y1 = vertices[index[2]].v - vertices[index[0]].v; // v1 - v0
				float x2 = vertices[index[1]].u - vertices[index[0]].u; // u2 - u0
				float y2 = vertices[index[1]].v - vertices[index[0]].v; // v2 - v0

				float div = (x1 * y2 - x2 * y1);

				if (div == 0)
				{
					continue;
				}

				float det = 1.0f / div; // ad - bc
				XMVECTOR t = (e1 * y2 - e2 * y1) * det;

				vertices[index[0]].m_Tangent += t;
				vertices[index[2]].m_Tangent += t;
				vertices[index[1]].m_Tangent += t;
			}
			else
			{
				meshInfo.m_subMeshs[facies[faceID].m_materialRef].push_back(index[0]);
				meshInfo.m_subMeshs[facies[faceID].m_materialRef].push_back(index[1]);
				meshInfo.m_subMeshs[facies[faceID].m_materialRef].push_back(index[2]);

				// e1 = p1 - p0, e2 = p2 - p0
				XMVECTOR p0 = { vertices[index[0]].m_pos.x, vertices[index[0]].m_pos.y, vertices[index[0]].m_pos.z };
				XMVECTOR p1 = { vertices[index[1]].m_pos.x, vertices[index[1]].m_pos.y, vertices[index[2]].m_pos.z };
				XMVECTOR p2 = { vertices[index[2]].m_pos.x, vertices[index[2]].m_pos.y, vertices[index[1]].m_pos.z };

				XMVECTOR e1 = p1 - p0;
				XMVECTOR e2 = p2 - p0;

				float x1 = vertices[index[1]].u - vertices[index[0]].u; // u1 - u0
				float y1 = vertices[index[1]].v - vertices[index[0]].v; // v1 - v0
				float x2 = vertices[index[2]].u - vertices[index[0]].u; // u2 - u0
				float y2 = vertices[index[2]].v - vertices[index[0]].v; // v2 - v0

				float div = (x1 * y2 - x2 * y1);

				if (div == 0)
				{
					continue;
				}

				float det = 1.0f / div; // ad - bc

				XMVECTOR t = (e1 * y2 - e2 * y1) * det;

				vertices[index[0]].m_Tangent += t;
				vertices[index[1]].m_Tangent += t;
				vertices[index[2]].m_Tangent += t;
			}
			break;
		}
		}
	}

	for (auto& idxList : m_SameVertexIndexLists)
	{
		for (int i = 1; i < (int)idxList.size(); i++)
		{
			vertices[idxList[i]].m_Tangent = vertices[idxList[0]].m_Tangent;
		}
	}
}

void ASEParse::ASEParser::ParsingMesh_TextureVerteies(vector<VertexInfo>& vertices, vector<FaceInfo>& facies, vector<Vector2>& uvs)
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MESH_TVERT:
		{
			Parsing_NumberInt();
			Vector3 uv3 = Parsing_NumberVector3();
			if (m_ChangeYZ)
			{
				uvs.push_back(Vector2(uv3.x, 1 - uv3.z));
			}
			else
			{
				uvs.push_back(Vector2(uv3.x, 1 - uv3.y));
			}
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingMesh_UVtoVertex(vector<VertexInfo>& vertices, vector<FaceInfo>& facies, vector<Vector2>& uvs)
{
	LONG nowtoken;
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MESH_TFACE:
		{
			int faceIdx = Parsing_NumberInt();
			int uvID[3];
			uvID[0] = Parsing_NumberInt();
			uvID[1] = Parsing_NumberInt();
			uvID[2] = Parsing_NumberInt();

			for (int i = 0; i < 3; i++)
			{
				// 텍스쳐 값이 설정 되어있지 않을 경우 UV 값을 설정
				if (!vertices[facies[faceIdx].m_vertexindex[i]].m_IsTextureSet)
				{
					vertices[facies[faceIdx].m_vertexindex[i]].u = uvs[uvID[i]].x;
					vertices[facies[faceIdx].m_vertexindex[i]].v = uvs[uvID[i]].y;

					vertices[facies[faceIdx].m_vertexindex[i]].m_IsTextureSet = true;
				}
				else if (std::abs(vertices[facies[faceIdx].m_vertexindex[i]].u - uvs[uvID[i]].x) > 0.00001 &&
					std::abs(vertices[facies[faceIdx].m_vertexindex[i]].v - uvs[uvID[i]].y) > 0.00001)
				{
					bool isNewUV = true;

					for (auto vertex : m_SameVertexIndexLists[facies[faceIdx].m_vertexindex[i]])
					{
						if (std::abs(vertices[vertex].u - uvs[uvID[i]].x) < 0.00001 &&
							std::abs(vertices[vertex].v - uvs[uvID[i]].y) < 0.00001)
						{
							isNewUV = false;
							facies[faceIdx].m_vertexindex[i] = vertex;
						}
					}

					if (isNewUV)
					{
						VertexInfo cVertex(vertices[facies[faceIdx].m_vertexindex[i]]);
						cVertex.u = uvs[uvID[i]].x;
						cVertex.v = uvs[uvID[i]].y;
						cVertex.m_IsTextureSet = true;
						vertices.push_back(cVertex);
						int newIndex = (int)vertices.size() - 1;
						m_SameVertexIndexLists[facies[faceIdx].m_vertexindex[i]].push_back(newIndex);
						facies[faceIdx].m_vertexindex[i] = newIndex;
					}
				}
			}
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingAnimation(ASEPrefabInfo& asePrefab, ASEObject& aseObject)
{
	LONG nowtoken;

	// Todo : 여러개의 애니메이션
	//aseObject.m_AnimationInfos.push_back(AnimationInfo());

	AnimationInfo& currAnimationInfo = aseObject.m_AnimationInfo;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_NODE_NAME:
		{
			currAnimationInfo.m_InfoName = Parsing_String();
			asePrefab.m_HasAnimation = true;
			aseObject.m_HasAnimationInfo = true;
			break;
		}
		case TOKENR_CONTROL_POS_TRACK:
		{
			ParsingAnimationPos(currAnimationInfo);
			break;
		}
		case TOKENR_CONTROL_ROT_TRACK:
		{
			ParsingAnimationRot(currAnimationInfo);
			break;
		}
		case TOKENR_CONTROL_SCALE_TRACK:
		{
			ParsingAnimationScale(currAnimationInfo);
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingAnimationPos(AnimationInfo& animInfo)
{
	LONG nowtoken;

	float startFrame = m_CurrSceneInfo.m_FirstFrame * m_CurrSceneInfo.m_TicksPerFrame;
	float tickPerSpeed = m_CurrSceneInfo.m_TicksPerFrame * m_CurrSceneInfo.m_FrameSpeed;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_CONTROL_POS_SAMPLE:
		{
			float frameRate = (Parsing_NumberFloat() - startFrame) / tickPerSpeed;
			Vector3 pos = Parsing_NumberVector3();

			animInfo.m_PosTrackList.push_back(PosTrack(frameRate, pos));

			if (animInfo.m_MinFrameRate > frameRate)
			{
				animInfo.m_MinFrameRate = frameRate;
			}
			else if (animInfo.m_MaxFrameRate < frameRate)
			{
				animInfo.m_MaxFrameRate = frameRate;
			}

			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingAnimationRot(AnimationInfo& animInfo)
{
	LONG nowtoken;
	Quaternion preQuaternion = Quaternion::Identity;

	float startFrame = m_CurrSceneInfo.m_FirstFrame * m_CurrSceneInfo.m_TicksPerFrame;
	float tickPerSpeed = m_CurrSceneInfo.m_TicksPerFrame * m_CurrSceneInfo.m_FrameSpeed;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			float frameRate = (Parsing_NumberFloat() - startFrame) / tickPerSpeed;

			Vector3 axis = Parsing_NumberVector3();
			float angle = Parsing_NumberFloat();

			Quaternion rot = Quaternion::CreateFromAxisAngle(axis, angle);

			XMVECTOR q1 = XMLoadFloat4(&rot);
			XMVECTOR q2 = XMLoadFloat4(&preQuaternion);
			XMVECTOR q = DirectX::XMQuaternionMultiply(q2, q1);

			XMStoreFloat4(&rot, q);

			Vector3 rotation;
			{

				double x, y, z, w;
				x = rot.x;
				y = rot.y;
				z = rot.z;
				w = rot.w;

				double sqx = x * x;
				double sqy = y * y;
				double sqz = z * z;
				double sqw = w * w;

				rotation.x = (float)(asin(2.0 * (x * w - y * z)));

				rotation.y = (float)(atan2(2.0f * (x * z + y * w), (-sqx - sqy + sqz + sqw)));

				rotation.z = (float)(atan2(2.0 * (x * y + z * w), (-sqx + sqy - sqz + sqw)));
			}

			animInfo.m_RotTrackList.push_back(RotTrack(frameRate, rot, rotation));

			if (animInfo.m_MinFrameRate > frameRate)
			{
				animInfo.m_MinFrameRate = frameRate;
			}
			else if (animInfo.m_MaxFrameRate < frameRate)
			{
				animInfo.m_MaxFrameRate = frameRate;
			}

			preQuaternion = rot;
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingAnimationScale(AnimationInfo& animInfo)
{
	LONG nowtoken;

	float startFrame = m_CurrSceneInfo.m_FirstFrame * m_CurrSceneInfo.m_TicksPerFrame;
	float tickPerSpeed = m_CurrSceneInfo.m_TicksPerFrame * m_CurrSceneInfo.m_FrameSpeed;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			float frameRate = (Parsing_NumberFloat() - startFrame) / m_CurrSceneInfo.m_TicksPerFrame;
			Vector3 scale = Parsing_NumberVector3();

			animInfo.m_ScaleTrackList.push_back(ScaleTrack(frameRate, scale));
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingBoneList(ASEPrefabInfo& asePrefab)
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_BONE:
		{
			int index = Parsing_NumberInt();
			asePrefab.m_BoneList[index].m_Index = index;


			nowtoken = m_lexer->GetToken(m_TokenString);
			nowtoken = m_lexer->GetToken(m_TokenString);

			if (nowtoken == TOKENR_BONE_NAME)
			{
				asePrefab.m_BoneList[index].m_Bone_Name = Parsing_String();
			}
			nowtoken = m_lexer->GetToken(m_TokenString);
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingVertexWeightList(ASEPrefabInfo& asePrefab)
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MESH_WEIGHT:
		{
			int vertexIndex = Parsing_NumberInt();
			Parsing_Weight(vertexIndex);
		}
		}
	}
}

void ASEParse::ASEParser::Parsing_Weight(int vertexIdx)
{
	LONG nowtoken;

	int i = 0;
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			float boneIdx = -1, weight = -1;
			boneIdx = Parsing_NumberFloat();
			weight = Parsing_NumberFloat();

			for (auto idx : m_SameVertexIndexLists[vertexIdx])
			{
				switch (i)
				{
				case 0:
				{
					meshInfo.m_vertices[idx].m_bones.x = boneIdx;
					meshInfo.m_vertices[idx].m_weights.x = weight;
					//m_LoadMesh->m_Vertices[idx].Bone.x = boneIdx;
					//m_LoadMesh->m_Vertices[idx].Weight.x = weight;
					break;
				}
				case 1:
				{
					meshInfo.m_vertices[idx].m_bones.y = boneIdx;
					meshInfo.m_vertices[idx].m_weights.y = weight;
					//m_LoadMesh->m_Vertices[idx].Bone.y = boneIdx;
					//m_LoadMesh->m_Vertices[idx].Weight.y = weight;
					break;
				}
				case 2:
				{
					meshInfo.m_vertices[idx].m_bones.z = boneIdx;
					meshInfo.m_vertices[idx].m_weights.z = weight;
					//m_LoadMesh->m_Vertices[idx].Bone.z = boneIdx;
					//m_LoadMesh->m_Vertices[idx].Weight.z = weight;
					break;
				}
				case 3:
				{
					meshInfo.m_vertices[idx].m_bones.w = boneIdx;
					meshInfo.m_vertices[idx].m_weights.w = weight;
					//m_LoadMesh->m_Vertices[idx].Bone.w = boneIdx;
					//m_LoadMesh->m_Vertices[idx].Weight.w = weight;
					break;
				}
				}
#ifdef _DEBUG
				assert(i < 3);
#endif
			}

			i++;
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingMarterialList()
{
	LONG nowtoken;
	m_MaterialIDs.clear();
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MATERIAL_COUNT:
		{
			break;
		}
		case TOKENR_MATERIAL:
		{
			ParsingMarterial();
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingMarterial()
{
	LONG nowtoken;

	TLGraphicsEngine::Material* newMat = nullptr;

	// 마테리얼 값 안쓰도록 변경되었음
	// Thansk Light는 ASE로 매쉬만 받아서 사용하도록 하기위해서

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MATERIAL_NAME:
		{
			/*	auto name = Parsing_String();
				newMat = new REngine::Material(name);
				m_MaterialIDs.push_back(name);*/
			break;
		}
		case TOKENR_NUMSUBMTLS:
		{
			meshInfo.m_subMeshs.resize(Parsing_NumberInt());
		}
		case TOKENR_MATERIAL_CLASS:
		{
			auto matClass = std::string(Parsing_String());
			/*if (matClass == "Standard")
			{
				newMat->m_VShaderID = "Deferred_VS_Mesh";
				newMat->m_PShaderID = "Deferred_PS";
			}
			else if (matClass == "Skinned")
			{
				newMat->m_VShaderID = "Deferred_VS_Skin";
				newMat->m_PShaderID = "Deferred_PS";
			}
			else if (matClass == "Debug")
			{
				newMat->m_VShaderID = "Deferred_VS_Mesh";
				newMat->m_PShaderID = "Deferred_Debug_PS";
			}
			else
			{
				delete newMat;
				return;
			}*/
			break;
		}
		case TOKENR_MATERIAL_AMBIENT:
		{
			/*newMat->m_MaterialCB.Ambient.x = Parsing_NumberFloat();
			newMat->m_MaterialCB.Ambient.y = Parsing_NumberFloat();
			newMat->m_MaterialCB.Ambient.z = Parsing_NumberFloat();*/
			break;
		}
		case TOKENR_MATERIAL_DIFFUSE:
		{
			//newMat->m_MaterialCB.Diffuse.x = Parsing_NumberFloat();
			//newMat->m_MaterialCB.Diffuse.y = Parsing_NumberFloat();
			//newMat->m_MaterialCB.Diffuse.z = Parsing_NumberFloat();
			break;
		}
		case TOKENR_MATERIAL_SPECULAR:
		{
			//newMat->m_MaterialCB.Specular.x = Parsing_NumberFloat();
			//newMat->m_MaterialCB.Specular.y = Parsing_NumberFloat();
			//newMat->m_MaterialCB.Specular.z = Parsing_NumberFloat();
			break;
		}
		case TOKENR_MAP_DIFFUSE:
		{
			//ParsingMapDiffuse(newMat);
			break;
		}
		case TOKENR_MAP_BUMP:
		{
			//ParsingMapBump(newMat);
			break;
		}
		case TOKENR_MAP_MR:
		{
			//ParsingMapMR(newMat);
			break;
		}
		}
	}

	//REngine::GraphicResourceManager::Instance()->RegistMaterialBuffer(newMat->m_Name, newMat);
}

void ASEParse::ASEParser::ParsingMapBump(TLGraphicsEngine::Material* mat)
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MAP_NAME:
		{
			break;
		}
		case TOKENR_MAP_GENERIC:
		{
			mat->m_PShaderID = "Deferred_PS_Bump";
			ParsingMapGeneric(mat);
			break;
		}
		case TOKENR_BITMAP:
		{
			auto file = std::string(Parsing_String());
			if (file != "None")
			{
				mat->m_bumpTexID = file;
			}
			else
			{
				mat->m_PShaderID = "Deferred_PS_NT";
			}
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingMapDiffuse(TLGraphicsEngine::Material* mat)
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MAP_NAME:
		{
			break;
		}
		case TOKENR_BITMAP:
		{
			auto file = std::string(Parsing_String());
			if (file != "None")
			{
				mat->m_mainTexID = file;
			}
			else
			{
				mat->m_PShaderID = "Deferred_PS_NT";
			}
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingMapMR(TLGraphicsEngine::Material* mat)
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MAP_NAME:
		{
			break;
		}
		case TOKENR_BITMAP:
		{
			auto file = std::string(Parsing_String());
			if (file != "None")
			{
				mat->m_metallicGlossMapID = file;
				mat->m_PShaderID = "Deferred_PS_PBR";
			}
			else
			{
#ifdef _DEBUG
				assert(true);
#endif // _DEBUG
				mat->m_PShaderID = "Deferred_PS_NT";
			}
			break;
		}
		}
	}
}

void ASEParse::ASEParser::ParsingMapGeneric(TLGraphicsEngine::Material* mat)
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		switch (nowtoken)
		{
		case TOKENR_MAP_NAME:
		{
			break;
		}
		case TOKENR_BITMAP:
		{
			auto file = std::string(Parsing_String());
			if (file != "None")
			{
				mat->m_mainTexID = file;
			}
			else
			{
				mat->m_PShaderID = "Deferred_PS_NT";
			}
			break;
		}
		}
	}
}