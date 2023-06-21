#include "pch.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "GraphicsEngine.h"
#include "GraphicResourceManager.h"
#include "EngineCamera.h"

using namespace TLGraphicsEngine; 

Material::Material(const char* name) :
	IMaterial(name)
{
	m_MaterialCB.Diffuse = { 1,1,1,1 };
	m_MaterialCB.Emissive = { 0,0,0,0 };
	m_fMetallic = 0.5;
	m_fRoughness = 0.5;
	m_tiling = { 1, 1 };
}

Material::~Material()
{
}

bool Material::SetShader(std::shared_ptr<Shader> pixelShader)
{
	if (pixelShader == nullptr)
	{
		// 잘못된 쉐이더를 대입중
		assert(false);
		return false;
	}

	m_pixelShader = pixelShader;

	return true;
}

void Material::SetTexture(const char* name, UID texture)
{

}

void Material::SetTexture(const char* name, std::shared_ptr<ITexture> texture)
{
	if (!strcmp(name, "_MainTex"))
	{
		m_mainTex = std::reinterpret_pointer_cast<Texture, ITexture>(texture);

		if (m_mainTex == nullptr)
		{
			m_SetAlbedoMap = false;
		}
		else
		{
			m_SetAlbedoMap = true;
		}
	}
	else if (!strcmp(name ,"_BumpTex"))
	{
		m_bumpTex = std::reinterpret_pointer_cast<Texture, ITexture>(texture);

		if (m_bumpTex == nullptr)
		{
			m_SetBumpMap = false;
		}
		else
		{
			m_SetBumpMap = true;
		}

	}
	else if (!strcmp(name, "_MetallicGlossMap"))
	{
		m_metallicGlossMap = std::reinterpret_pointer_cast<Texture, ITexture>(texture);;
	}
	else if (!strcmp(name, "_OcclusionMap"))
	{
		m_occlusionMap = std::reinterpret_pointer_cast<Texture, ITexture>(texture);;
	}
}

void Material::SetInt(const char* name, int value)
{

}

void Material::SetFloat(const char* name, float value)
{
	if (!strcmp(name, "_Smoothness"))
	{
		m_fRoughness = 1.f - value;
	}
	else if (!strcmp(name, "_Metallic"))
	{
		m_fMetallic = value;
	}
}
void Material::SetColor(const char* name, DirectX::SimpleMath::Color value)
{
	m_color = value;
}

void Material::SetVector(const char* name, DirectX::SimpleMath::Vector4 value)
{
	if (!strcmp(name, "m_Scale"))
	{
		m_tiling.x = value.x;
		m_tiling.y = value.y;
	}
	else if(!strcmp(name, "m_Offset"))
	{
		m_tiling.x = value.x;
		m_tiling.y = value.y;
	}
}

void Material::SetEmissiveColor(DirectX::SimpleMath::Color value)
{
	m_MaterialCB.Emissive = value;
}

void Material::SetAlbedoColor(DirectX::SimpleMath::Color value)
{
	m_MaterialCB.Diffuse = value;
}

void Material::RemoveConstBuffer()
{
	m_setConstBuffer = false;
}

void Material::SetConstBuffer(void* constBuffer, int size)
{
	m_setConstBuffer = true;

	if (sizeof(m_perCustumConstBuffer) > size)
	{
		// 크기가 큰것이 들어왔다 죄악의 크기를 더 늘리자
		assert(false);
	}

	memcpy(&m_perCustumConstBuffer, constBuffer, size);
}
