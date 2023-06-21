#include "pch.h"
#include "DebugMesh.h"
#include "GraphicsEngine.h"
#include "EngineCamera.h"

TLGraphicsEngine::DebugMesh::DebugMesh() : Mesh("DebugMesh")
{
	m_Primitive = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

TLGraphicsEngine::DebugMesh::~DebugMesh()
{

}

void TLGraphicsEngine::DebugMesh::MakeCameraMesh()
{
	float _near = GraphicsEngine::Instance()->GetEngineCamera()->GetNear();
	float _far = GraphicsEngine::Instance()->GetEngineCamera()->GetFar();
	float _fovY = 1.5708f;//GraphicsEngine::Instance()->GetEngineCamera()->m_FovY;
	float _Aspect = GraphicsEngine::Instance()->GetEngineCamera()->GetAspect();
	_far = 20.0f;
	_near = 0.1f;

	float nearHeight = _near * tanf(0.5f * _fovY);
	float farHeight = _far * tanf(0.5f * _fovY);
	float nearWidth = nearHeight * _Aspect;
	float farWidth = farHeight * _Aspect;

	// Vertex
	std::vector<Vertex> vtTemp =
	{
		{ DirectX::XMFLOAT3(-nearWidth, -nearHeight, _near),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-nearWidth, nearHeight, _near),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(nearWidth,  nearHeight, _near),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(nearWidth, -nearHeight, _near),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-farWidth, -farHeight, _far),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-farWidth, farHeight, _far),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(farWidth, farHeight, _far),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(farWidth, -farHeight, _far),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
	};

	// Index
	std::vector<UINT> indTemp =
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};


	SetVertices(vtTemp);
	SetTriangles(indTemp);
}

void TLGraphicsEngine::DebugMesh::MakeGridMesh()
{
	// Vertex
	std::vector<Vertex> vtTemp =
	{
		// LEFT
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +19.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +18.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +17.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +16.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +15.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +14.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +13.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +12.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +11.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +10.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +09.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +08.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +07.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +06.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +05.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +04.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +03.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +02.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +01.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +00.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -01.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -02.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -03.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -04.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -05.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -06.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -07.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -08.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -09.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -10.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -11.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -12.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -13.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -14.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -15.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -16.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -17.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -18.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -19.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },

		// RIGHT
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +19.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +18.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +17.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +16.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +15.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +14.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +13.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +12.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +11.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +10.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +09.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +08.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +07.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +06.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +05.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +04.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +03.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +02.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +01.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +00.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -01.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -02.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -03.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -04.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -05.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -06.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -07.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -08.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -09.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -10.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -11.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -12.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -13.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -14.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -15.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -16.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -17.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -18.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -19.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },

		// Top
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+19.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+18.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+17.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+16.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+15.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+14.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+13.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+12.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+11.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+10.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+09.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+08.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+07.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+06.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+05.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+04.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+03.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+02.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+01.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+00.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-01.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-02.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-03.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-04.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-05.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-06.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-07.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-08.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-09.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-10.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-11.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-12.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-13.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-14.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-15.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-16.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-17.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-18.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-19.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, +20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },

		// Bottom
		{ DirectX::XMFLOAT3(+20.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+19.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+18.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+17.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+16.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+15.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+14.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+13.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+12.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+11.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+10.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+09.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+08.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+07.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+06.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+05.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+04.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+03.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+02.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+01.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+00.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-01.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-02.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-03.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-04.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-05.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-06.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-07.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-08.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-09.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-10.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-11.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-12.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-13.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-14.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-15.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-16.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-17.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-18.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-19.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-20.0f, +0.0f, -20.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
	};

	// Index
	std::vector<UINT> indTemp =
	{
		// row
		0,	41,
		1,	42,
		2,	43,
		3,	44,
		4,	45,
		5,	46,
		6,	47,
		7,	48,
		8,	49,
		9,	50,
		10,	51,
		11,	52,
		12,	53,
		13,	54,
		14,	55,
		15,	56,
		16,	57,
		17,	58,
		18,	59,
		19,	60,
		20,	61,
		21,	62,
		22,	63,
		23,	64,
		24,	65,
		25,	66,
		26,	67,
		27,	68,
		28,	69,
		29,	70,
		30,	71,
		31,	72,
		32,	73,
		33,	74,
		34,	75,
		35,	76,
		36,	77,
		37,	78,
		38,	79,
		39,	80,
		40,	81,

		// column
		82, 123,
		83, 124,
		84, 125,
		85, 126,
		86, 127,
		87, 128,
		88, 129,
		89, 130,
		90, 131,
		91, 132,
		92, 133,
		93, 134,
		94, 135,
		95, 136,
		96, 137,
		97, 138,
		98, 139,
		99, 140,
		100, 141,
		101, 142,
		102, 143,
		103, 144,
		104, 145,
		105, 146,
		106, 147,
		107, 148,
		108, 149,
		109, 150,
		110, 151,
		111, 152,
		112, 153,
		113, 154,
		114, 155,
		115, 156,
		116, 157,
		117, 158,
		118, 159,
		119, 160,
		120, 161,
		121, 162,
		122, 163,
	};

	SetVertices(vtTemp);
	SetTriangles(indTemp);
}

void TLGraphicsEngine::DebugMesh::MakeAxisMesh()
{
	// Vertex
	std::vector<Vertex> vtTemp =
	{
		// X Axis
		{ DirectX::XMFLOAT3(0.0f, +0.0f, +0.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+50.0f, +0.0f, +0.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		// Y Axis
		{ DirectX::XMFLOAT3(+0.0f,   0.0f, +0.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+0.0f, +50.0f, +0.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

		// Z Axis
		{ DirectX::XMFLOAT3(+0.0f, +0.0f,   0.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+0.0f, +0.0f, +50.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	};

	// Index
	std::vector<UINT> indTemp =
	{
		// X Axis
		0, 1,

		// Y Axis
		2, 3,

		// Z Axis
		4, 5
	};

	SetVertices(vtTemp);
	SetTriangles(indTemp);
}

void TLGraphicsEngine::DebugMesh::MakeBoundingBoxMesh()
{
	// Vertex
	std::vector<Vertex> vtTemp =
	{
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	// Index
	std::vector<UINT> indTemp =
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	SetVertices(vtTemp);
	SetTriangles(indTemp);

	m_Primitive = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
}

void TLGraphicsEngine::DebugMesh::MakeBoxColliderMesh()
{
	// Vertex
	std::vector<Vertex> vtTemp =
	{
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }
	};

	// Index
	std::vector<UINT> indTemp =
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	SetVertices(vtTemp);
	SetTriangles(indTemp);

	m_Primitive = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
}

void TLGraphicsEngine::DebugMesh::MakePlane()
{
	// Vertex
	std::vector<Vertex> vtTemp =
	{
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	// Index
	std::vector<UINT> indTemp =
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	SetVertices(vtTemp);
	SetTriangles(indTemp);
}