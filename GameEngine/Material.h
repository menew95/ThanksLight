#pragma once

#include "Object.h"

#include "MaterialData.h"
namespace TLGraphicsEngine
{
class IMaterial;
class Shader;
}

namespace TLGameEngine
{
class Shader;
class Texture;

struct MaterialData
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector4 reflect;
};

class Material : public Common::Object
{
public:
	Material();
	virtual ~Material() { }

	void SetShader(std::shared_ptr<Shader> shader);
	void SetTexture(std::shared_ptr<Texture> texture);
	void SetBumpTexture(std::shared_ptr<Texture> texture);
	void SetSmoothness(float roughness);
	void SetMetallic(float metallic);
	void SetColor(Color color);
	void SetEmissiveColor(Color color);
	void SetTiling(Vector2 value);
	void SetIMaterial(std::shared_ptr<TLGraphicsEngine::IMaterial> material);
	void SetData(const MaterialData& data);

	void SetCustomBuffer(void* data, int size);
	std::shared_ptr<TLGraphicsEngine::IMaterial> GetIMaterial() { return m_iMaterial; }
private:
	void PushData();

	std::shared_ptr<Shader> m_shader;

	std::shared_ptr<MaterialData> m_data;

	std::shared_ptr<TLGraphicsEngine::IMaterial> m_iMaterial;
};
}
