#pragma once

#include "Component.h"

namespace TLGameEngine
{
class MeshFilter : public Component
{
public:
	MeshFilter();

	const UID& GetMeshUID() { return m_Mesh.guid; }

private:
	FileInfo m_Mesh;

	BOOST_DESCRIBE_CLASS(TLGameEngine::MeshFilter, (), (), (), (m_Mesh))
};
}