#include "pch.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "IMesh.h"

void TLGameEngine::Mesh::SetVertices(const std::vector<Vertex>& verteices)
{
	//Renderer::Instance().GetRenderer()->SetVertices(&verteices[0], sizeof(Vertex), static_cast<size_t>(verteices.size()), GetUID());
}

void TLGameEngine::Mesh::SetIndeices(const std::vector<size_t>& indices)
{
	//Renderer::Instance().GetRenderer()->SetIndices(&indices[0], sizeof(size_t), static_cast<size_t>(indices.size()), GetUID());
}

void TLGameEngine::Mesh::SetIMesh(std::shared_ptr<TLGraphicsEngine::IMesh> iMesh)
{
	m_iMesh = iMesh;
}
