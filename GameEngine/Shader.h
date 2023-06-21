#pragma once

#include "Object.h"

namespace TLGraphicsEngine
{
class Shader;
}

namespace TLGameEngine
{
class Shader : public Common::Object
{
public:
	virtual ~Shader() {}
	
	void SetIShader(std::shared_ptr<TLGraphicsEngine::Shader> shader) { m_shader = shader; }
	std::shared_ptr<TLGraphicsEngine::Shader> GetIShader() { return m_shader; }
private:
	std::shared_ptr<TLGraphicsEngine::Shader> m_shader;
};
}

