#pragma once
#include "Lexer.h"

enum MaterialToken
{
	m_Material			= 0,
	m_Name				= 1,
	m_VShader			= 2,
	m_PShader			= 3,
	m_SavedProperties	= 4,
	serializedVersion	= 5,
	m_TexEnvs			= 6,
	_MainMap			= 7,
	_NormalMap			= 8,
	m_Texture			= 9,
	m_Ambient			= 10,
	m_Diffuse			= 11,
	m_Specular			= 12,
	m_Reflect			= 13,
	m_Emissive			= 14,
	m_Metallic			= 15,
	m_Roughness			= 16
};

namespace MaterialParse
{
	// ASE¿ë Lexer
	class MaterialLexer : public CLexer
	{
	public:
		MaterialLexer(LPSTR p_FileName = NULL, DWORD p_SizeOfBuffer = MAX_SIZE_OF_BUFFER);
		virtual ~MaterialLexer();

		virtual	LONG GetToken(LPSTR p_TokenString);
	};
}