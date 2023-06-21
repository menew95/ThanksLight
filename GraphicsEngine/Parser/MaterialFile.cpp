#include "Materialfile.h"

#define	NUMBER_OF_RESERVEDWORD				17

const char* MatToken[NUMBER_OF_RESERVEDWORD] =
{
	"Material"				/*  0*/
,	"m_Name"				/*  1*/
,	"m_VShader"				/*  2*/
,	"m_PShader"				/*  3*/
,	"m_SavedProperties"		/*  4*/
,	"serializedVersion"		/*  5*/
,	"m_TexEnvs"				/*  6*/
,	"_MainMap"				/*  7*/
,	"_NormalMap"			/*  8*/
,	"m_Texture"				/*  9*/
,	"m_Ambient"				/*  10*/
,	"m_Diffuse"				/*  11*/
,	"m_Specular"			/*  12*/
,	"m_Reflect"				/*  13*/
,	"m_Emissive"			/*  14*/
,	"m_Metallic"			/*  15*/
,	"m_Roughness"			/*  16*/
};

MaterialParse::MaterialLexer::MaterialLexer(LPSTR p_FileName /*= NULL*/, DWORD p_SizeOfBuffer /*= MAX_SIZE_OF_BUFFER*/)
{

}

MaterialParse::MaterialLexer::~MaterialLexer()
{

}

LONG MaterialParse::MaterialLexer::GetToken(LPSTR p_TokenString)
{
	// 1) 앞쪽 쓸데없는 문자찾기
		//    앞쪽에 공백이나 Tab이나 개행이나 캐리지리턴 같은 문자열이 있으면 무시한다.
	CHAR	Char = GetNextChar();
	if (!Char)
	{
		return	TOKEND_END;
	}

	LONG	p_StringPoint = 0;
	while (Char == ' ' || Char == '\t' || Char == '\r' || Char == '\n')
	{
		Char = GetNextChar();
		if (!Char)
		{
			return	TOKEND_END;
		}
	}

	if (Char >= '0' && Char <= '9' || Char == '-')
	{
		LONG	p_StringPoint = 0;
		while ((Char >= '0' && Char <= '9') || Char == '-' || Char == '.')
		{
			p_TokenString[p_StringPoint] = Char;

			Char = GetNextChar();
			if (!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint] = NULL;


		return	TOKEND_NUMBER;
	}
	else if (Char == '"')
	{
		Char = GetNextChar();
		if (!Char)
		{
			return	TOKEND_END;
		}

		LONG	p_StringPoint = 0;
		while (Char != '"')
		{
			p_TokenString[p_StringPoint] = Char;

			Char = GetNextChar();
			if (!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint] = NULL;

		return	TOKEND_STRING;
	}
	else
	{
		LONG	p_StringPoint = 0;
		while (Char != ' ' && Char != '\t' && Char != '\r' && Char != '\n' && Char != '"' && Char != '{' && Char != '}' && Char != ':')
		{
			p_TokenString[p_StringPoint] = Char;

			Char = GetNextChar();
			if (!Char)
			{
				return	TOKEND_END;
			}
			p_StringPoint++;
		}
		p_TokenString[p_StringPoint] = NULL;

		for (int i = 0; i < NUMBER_OF_RESERVEDWORD; i++)
		{
			if (!strcmp(p_TokenString, MatToken[i]))
			{
				// 맞는 Token을 찾았다! 그 번호를 return시켜준다.
				return	i;
			}
		}

		// 맞는 것은 전혀 찾지 못했다. 정의되지 않은 TOKEN이라고 돌려준다.
		return	TOKEND_NOTDEFINED;
	}
}

