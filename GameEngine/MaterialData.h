#pragma once


namespace TLGameEngine
{
struct MaterialPropertyTexture
{
	FileInfo m_Texture;
};
BOOST_DESCRIBE_STRUCT(TLGameEngine::MaterialPropertyTexture, (), (m_Texture));

struct MaterialTexEnv
{
	MaterialPropertyTexture _MainTex;
	MaterialPropertyTexture _BumpMap;
};
BOOST_DESCRIBE_STRUCT(TLGameEngine::MaterialTexEnv, (), ());

struct MaterialSavedProperties
{
	std::vector<MaterialTexEnv> m_TexEnvs;
	std::vector<Color> m_Colors;
};
BOOST_DESCRIBE_STRUCT(TLGameEngine::MaterialSavedProperties, (), (m_TexEnvs));

struct MaterialParsingData
{
	std::string m_Name;
	MaterialSavedProperties m_SavedProperties;
};
BOOST_DESCRIBE_STRUCT(TLGameEngine::MaterialParsingData, (), (m_Name, m_SavedProperties));
}

