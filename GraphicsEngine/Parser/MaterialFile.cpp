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
	// 1) ���� �������� ����ã��
		//    ���ʿ� �����̳� Tab�̳� �����̳� ĳ�������� ���� ���ڿ��� ������ �����Ѵ�.
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
				// �´� Token�� ã�Ҵ�! �� ��ȣ�� return�����ش�.
				return	i;
			}
		}

		// �´� ���� ���� ã�� ���ߴ�. ���ǵ��� ���� TOKEN�̶�� �����ش�.
		return	TOKEND_NOTDEFINED;
	}
}

