#pragma once
#include <windows.h>
#include "Parser.h"

#include "MaterialFile.h"

namespace TLGraphicsEngine
{
	class Material;
}

namespace MaterialParse
{
	class MaterialParser :
		public Parser
	{
	public:
		MaterialParser();
		virtual ~MaterialParser();

	private:
		MaterialLexer* m_lexer = nullptr;

		char m_TokenString[256] = "";
		int m_Token = 0;
		int m_parsingmode = 0;

	public:
		// 초기화
		virtual bool Initialize();

		bool Load(const LPSTR filePath, TLGraphicsEngine::Material& newMat);

	private:
		int Parsing_NumberLong();			// long을 읽어서 리턴해준다.
		float Parsing_NumberFloat();		// float
		LPSTR Parsing_String();				// string
		int Parsing_NumberInt();			// int

	private:
		void ParsingMaterial(TLGraphicsEngine::Material& newMat);
	};
}
