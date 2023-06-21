#pragma once

#include "Object.h"

namespace TLGraphicsEngine
{
class IMesh;
}

namespace TLGameEngine
{
class Mesh : public Common::Object
{
public:
	virtual ~Mesh() {}

	void SetVertices(const std::vector<Vertex>& verteices);
	void SetIndeices(const std::vector<size_t>& indices);
	void SetIMesh(std::shared_ptr<TLGraphicsEngine::IMesh> iMesh);

	std::shared_ptr<TLGraphicsEngine::IMesh> GetIMesh() { return m_iMesh; }
private:
	std::wstring m_name;

	std::shared_ptr<TLGraphicsEngine::IMesh> m_iMesh;
};
}

