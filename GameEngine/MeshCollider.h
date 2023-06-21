#pragma once
#include "Collider.h"

namespace TLGameEngine
{
class MeshCollider : public Collider
{
public:
	MeshCollider();
	virtual ~MeshCollider();

	std::string GetMeshID() const { return m_Mesh.guid; }

private:
	FileInfo m_Mesh;

	BOOST_DESCRIBE_CLASS(TLGameEngine::MeshCollider, (), (), (), (m_Mesh))
};
}

