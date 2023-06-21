#include "pch.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "GameEngine.h"
#include "IGraphicsEngine.h"
#include "ITexture.h"
#include "IMaterial.h"

TLGameEngine::Material::Material()
{
	m_data = std::make_shared<MaterialData>();
	
}

void TLGameEngine::Material::SetShader(std::shared_ptr<TLGameEngine::Shader> shader)
{
	m_iMaterial->SetShader(shader->GetIShader());
}
void TLGameEngine::Material::SetTexture(std::shared_ptr<TLGameEngine::Texture> texture)
{
	if (texture == nullptr)
		return;
	m_iMaterial->SetTexture("_MainTex", texture->GetITexture());
}

void TLGameEngine::Material::SetMetallic(float metallic)
{
	if (m_iMaterial == nullptr)
		return;
	m_iMaterial->SetFloat("_Metallic", metallic);
}

void TLGameEngine::Material::SetBumpTexture(std::shared_ptr<Texture> texture)
{
	if (texture == nullptr)
		return;
	m_iMaterial->SetTexture("_BumpTex", texture->GetITexture());
}

void TLGameEngine::Material::SetSmoothness(float smoothness)
{
	if (m_iMaterial == nullptr)
		return;
	m_iMaterial->SetFloat("_Smoothness", smoothness);
}

void TLGameEngine::Material::SetColor(Color color)
{
	if (m_iMaterial == nullptr)
		return;
	
	DirectX::SimpleMath::Color _color{ color.R(), color.G(), color.B(), color.A() };

	m_iMaterial->SetAlbedoColor(_color);
}

void TLGameEngine::Material::SetEmissiveColor(Color color)
{
	if (m_iMaterial == nullptr)
		return;

	DirectX::SimpleMath::Color _color{ color.R(), color.G(), color.B(), color.A() };

	m_iMaterial->SetEmissiveColor(_color);
}

void TLGameEngine::Material::SetTiling(Vector2 value)
{
	if (m_iMaterial == nullptr)
		return;

	m_iMaterial->SetVector("m_Scale", {value.x, value.y, 1, 1 });
}

void TLGameEngine::Material::SetIMaterial(std::shared_ptr<TLGraphicsEngine::IMaterial> material)
{
	m_iMaterial = material;
}

void TLGameEngine::Material::SetData(const MaterialData& data)
{
	*m_data = data;
	PushData();
}

void TLGameEngine::Material::SetCustomBuffer(void* data, int size)
{
	m_iMaterial->SetConstBuffer(data, size);
}

void TLGameEngine::Material::PushData()
{
	//Renderer::Instance().GetRenderer()->SetMaterialData(_data.get(), sizeof(*_data), GetUID());
}
