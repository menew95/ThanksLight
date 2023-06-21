#include "MaterialParser.h"
#include "../Material.h"

using namespace MaterialParse;

MaterialParser::MaterialParser()
{

}

MaterialParser::~MaterialParser()
{
	if (m_lexer != nullptr)
	{
		delete m_lexer;
		m_lexer = nullptr;
	}
}

bool MaterialParser::Initialize()
{
	m_lexer = new MaterialParse::MaterialLexer;

	return TRUE;
}

bool MaterialParser::Load(const LPSTR filePath,  TLGraphicsEngine::Material& newMat)
{
	/// 0) 파일을 로드한다.
	if (!m_lexer->Open(filePath))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	ParsingMaterial(newMat);

	m_lexer->Close();

	return true;
}

int MaterialParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

float MaterialParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

LPSTR MaterialParser::Parsing_String()
{
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}

int MaterialParser::Parsing_NumberInt()
{
	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}

void MaterialParser::ParsingMaterial( TLGraphicsEngine::Material& newMat)
{
	LONG nowtoken;

	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_END)
	{
		switch (nowtoken)
		{
			case m_Material:
			{
				break;
			}
			case m_Name:
			{
				newMat.m_MatName = Parsing_String();
				break;
			}
			case m_VShader:
			{
				newMat.m_VShaderID = Parsing_String();
				break;
			}
			case m_PShader:
			{
				newMat.m_PShaderID = Parsing_String();
				break;
			}
			case m_SavedProperties:
			{
				break;
			}
			case serializedVersion:
			{
				break;
			}
			case m_TexEnvs:
			{
				break;
			}
			case _MainMap:
			{
				if (m_lexer->GetToken(m_TokenString) == m_Texture)
				{
					newMat.m_mainTexID = Parsing_String();
				}
				break;
			}
			case _NormalMap:
			{
				if (m_lexer->GetToken(m_TokenString) == m_Texture)
				{
					newMat.m_SetBumpMap = true;
					newMat.m_bumpTexID = Parsing_String();
				}
				break;
			}
			case m_Ambient:
			{
				newMat.m_MaterialCB.Ambient.x = Parsing_NumberFloat();
				newMat.m_MaterialCB.Ambient.y = Parsing_NumberFloat();
				newMat.m_MaterialCB.Ambient.z = Parsing_NumberFloat();
				newMat.m_MaterialCB.Ambient.w = Parsing_NumberFloat();
				break;
			}
			case m_Diffuse:
			{
				newMat.m_MaterialCB.Diffuse.x = Parsing_NumberFloat();
				newMat.m_MaterialCB.Diffuse.y = Parsing_NumberFloat();
				newMat.m_MaterialCB.Diffuse.z = Parsing_NumberFloat();
				newMat.m_MaterialCB.Diffuse.w = Parsing_NumberFloat();
				break;
			}
			case m_Specular:
			{
				newMat.m_MaterialCB.Specular.x = Parsing_NumberFloat();
				newMat.m_MaterialCB.Specular.y = Parsing_NumberFloat();
				newMat.m_MaterialCB.Specular.z = Parsing_NumberFloat();
				newMat.m_MaterialCB.Specular.w = Parsing_NumberFloat();
				break;
			}
			case m_Reflect:
			{
				newMat.m_MaterialCB.Reflect.x = Parsing_NumberFloat();
				newMat.m_MaterialCB.Reflect.y = Parsing_NumberFloat();
				newMat.m_MaterialCB.Reflect.z = Parsing_NumberFloat();
				newMat.m_MaterialCB.Reflect.w = Parsing_NumberFloat();
				break;
			}
			case m_Emissive:
			{
				newMat.m_MaterialCB.Emissive.x = Parsing_NumberFloat();
				newMat.m_MaterialCB.Emissive.y = Parsing_NumberFloat();
				newMat.m_MaterialCB.Emissive.z = Parsing_NumberFloat();
				newMat.m_MaterialCB.Emissive.w = Parsing_NumberFloat();
				break;
			}
			case m_Metallic:
			{
				newMat.m_fMetallic = Parsing_NumberFloat();
				break;
			}
			case m_Roughness:
			{
				newMat.m_fRoughness = Parsing_NumberFloat();
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