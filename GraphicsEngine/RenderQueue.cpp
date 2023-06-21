#include "pch.h"
#include "RenderQueue.h"

TLGraphicsEngine::RenderQueue::RenderQueue()
{

}

TLGraphicsEngine::RenderQueue::~RenderQueue()
{

}

void TLGraphicsEngine::RenderQueue::RegistMesh(Mesh* mesh, std::vector<Material*> mats, float WM[16])
{
	DirectX::XMMATRIX world = DirectX::XMMatrixSet(
		WM[0], WM[1], WM[2], WM[3], 
		WM[4], WM[5], WM[6], WM[7], 
		WM[8], WM[9], WM[10], WM[11], 
		WM[12], WM[13], WM[14], WM[15]
	);
	m_BegineRenderQueue.push(RenderObject(RenderObject::RenderType::Mesh, mesh, mats, world));
}

void TLGraphicsEngine::RenderQueue::RegistProjectedMesh(Mesh* mesh, std::vector<Material*> mat, float WM[16])
{
	DirectX::XMMATRIX world = DirectX::XMMatrixSet(
		WM[0], WM[1], WM[2], WM[3],
		WM[4], WM[5], WM[6], WM[7],
		WM[8], WM[9], WM[10], WM[11],
		WM[12], WM[13], WM[14], WM[15]
	);
	m_BegineRenderQueue.push(RenderObject(RenderObject::RenderType::ProjectedMesh, mesh, mat, world));
}

void TLGraphicsEngine::RenderQueue::RegistSkinnedMesh(Mesh* mesh, std::vector<Material*> mats, float WM[16], float* boneTMList, int size)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixSet(
		WM[0], WM[1], WM[2], WM[3],
		WM[4], WM[5], WM[6], WM[7],
		WM[8], WM[9], WM[10], WM[11],
		WM[12], WM[13], WM[14], WM[15]
	);
	m_BegineRenderQueue.push(RenderObject(RenderObject::RenderType::SkinnedMesh, mesh, mats, world, boneTMList, size));
}

void TLGraphicsEngine::RenderQueue::RegistGizmo(Mesh* mesh, float WM[16], XMFLOAT4 color)
{

	DirectX::XMMATRIX world = DirectX::XMMatrixSet(
		WM[0], WM[1], WM[2], WM[3],
		WM[4], WM[5], WM[6], WM[7],
		WM[8], WM[9], WM[10], WM[11],
		WM[12], WM[13], WM[14], WM[15]
	);
	m_DrawGizmoQueue.push(GizmoObject(mesh, world, color));
}

void TLGraphicsEngine::RenderQueue::RegistOutLine(Mesh* mesh, float WM[16])
{
	DirectX::XMMATRIX world = DirectX::XMMatrixSet(
		WM[0], WM[1], WM[2], WM[3],
		WM[4], WM[5], WM[6], WM[7],
		WM[8], WM[9], WM[10], WM[11],
		WM[12], WM[13], WM[14], WM[15]
	);

	m_BegineRenderQueue.push(RenderObject(RenderObject::RenderType::OutLine, mesh, world));
}

void TLGraphicsEngine::RenderQueue::RegistUI(float x, float y, DirectX::XMFLOAT4 color, TCHAR* text)
{

}
