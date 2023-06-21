#include "pch.h"
#include "Frustum.h"
#include "EngineCamera.h"
#include "Mesh.h"

TLGraphicsEngine::Frustum::Frustum(EngineCamera* engineCamera) : m_EngineCamera(engineCamera)
{
	for (int i = 0; i < 6; i++)
	{
		m_Planes[i] = DirectX::XMVectorZero();
	}
	m_TestFrustumCamWorld = DirectX::XMMatrixIdentity();
	m_TestFrustumView = DirectX::XMMatrixIdentity();
	m_TestFrustumProj = DirectX::XMMatrixIdentity();
}

TLGraphicsEngine::Frustum::~Frustum()
{

}

void TLGraphicsEngine::Frustum::FrustumUpdate(float posx, float posy, float posz, float forwardx,
float forwardy, float forwardz, float upx, float upy, float upz,
float fovY, float aspectRatio, float nearZ, float farZ)
{
	DirectX::XMVECTOR eye, foc, up, right;

	eye = DirectX::XMVectorSet(posx, posy, posz, 0.f);
	foc = DirectX::XMVectorSet(forwardx, forwardy, forwardz, 0.f);
	up = DirectX::XMVectorSet(upx, upy, upz, 0.f);

	right = DirectX::XMVector3Cross(up, foc);

	right = DirectX::XMVector3Normalize(right);

	m_TestFrustumView = DirectX::XMMatrixLookAtLH(eye, DirectX::XMVectorAdd(eye, foc), up);
	m_TestFrustumProj = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);

	Update(0, m_TestFrustumProj, m_TestFrustumView);
}

void TLGraphicsEngine::Frustum::Update(float screenDepth, XMMATRIX proj, XMMATRIX view)
{
	// 뷰 매쉬의 버텍스가 이미 카메라 좌표계 임으로 Proj 행렬만 사용하여 평면을 만든다.
	XMMATRIX viewProj = /* view **/ proj;// m_EngineCamera->GetViewProjTM();

	//// left
	m_Planes[1].vector4_f32[0] = viewProj._14 + viewProj._11;
	m_Planes[1].vector4_f32[1] = viewProj._24 + viewProj._21;
	m_Planes[1].vector4_f32[2] = viewProj._34 + viewProj._31;
	m_Planes[1].vector4_f32[3] = viewProj._44 + viewProj._41;

	// right
	m_Planes[0].vector4_f32[0] = viewProj._14 - viewProj._11;
	m_Planes[0].vector4_f32[1] = viewProj._24 - viewProj._21;
	m_Planes[0].vector4_f32[2] = viewProj._34 - viewProj._31;
	m_Planes[0].vector4_f32[3] = viewProj._44 - viewProj._41;

	// top
	m_Planes[2].vector4_f32[0] = viewProj._14 - viewProj._12;
	m_Planes[2].vector4_f32[1] = viewProj._24 - viewProj._22;
	m_Planes[2].vector4_f32[2] = viewProj._34 - viewProj._32;
	m_Planes[2].vector4_f32[3] = viewProj._44 - viewProj._42;

	// bot
	m_Planes[3].vector4_f32[0] = viewProj._14 + viewProj._12;
	m_Planes[3].vector4_f32[1] = viewProj._24 + viewProj._22;
	m_Planes[3].vector4_f32[2] = viewProj._34 + viewProj._32;
	m_Planes[3].vector4_f32[3] = viewProj._44 + viewProj._42;

	// near
	m_Planes[4].vector4_f32[0] = viewProj._13;
	m_Planes[4].vector4_f32[1] = viewProj._23;
	m_Planes[4].vector4_f32[2] = viewProj._33;
	m_Planes[4].vector4_f32[3] = viewProj._43;

	// far
	m_Planes[5].vector4_f32[0] = viewProj._14 - viewProj._13;
	m_Planes[5].vector4_f32[1] = viewProj._24 - viewProj._23;
	m_Planes[5].vector4_f32[2] = viewProj._34 - viewProj._33;
	m_Planes[5].vector4_f32[3] = viewProj._44 - viewProj._43;

	for (int i = 0; i < 6; i++)
	{
		m_Planes[i] = DirectX::XMPlaneNormalize(m_Planes[i]);
	}
	
}

XMFLOAT3* TLGraphicsEngine::Frustum::Setting(XMFLOAT3(&boundingVertex)[8], Mesh* mesh/*, Mesh* debugMesh*/, XMMATRIX meshWorld)
{
	XMMATRIX viewProj = m_TestFrustumView * m_TestFrustumProj;

	for (int i = 0; i < 8; i++)
	{
		XMVECTOR v1 = DirectX::XMLoadFloat3(&mesh->m_BoundingBoxVertices[i].Pos);
		XMVECTOR X = DirectX::XMVector3Transform(v1, meshWorld * viewProj);
		DirectX::XMStoreFloat3(&boundingVertex[i], X);
	}

	return boundingVertex;
}

bool TLGraphicsEngine::Frustum::CheckPoint(float x, float y, float z)
{
	using namespace DirectX;
	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet(x, y, z, 1.0f))) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool TLGraphicsEngine::Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	using namespace DirectX;
	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		
		return false;
	}

	return true;
}

bool TLGraphicsEngine::Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	using namespace DirectX;
	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet(xCenter, yCenter, zCenter, 1.0f))) < -radius)
		{
			return false;
		}
	}

	return true;
}

bool TLGraphicsEngine::Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	using namespace DirectX;
	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool TLGraphicsEngine::Frustum::CheckBoundingBox(XMFLOAT3 pos[8])
{
	using namespace DirectX;
	for (int i = 0; i < 6; i++)
	{
		bool next = false;
		for (int j = 0; j < 8; j++)
		{
			XMVECTOR temp = XMPlaneDotCoord(m_Planes[i], XMVectorSet(pos[j].x, pos[j].y, pos[j].z, 1.0f));
			if (XMVectorGetX(XMPlaneDotCoord(m_Planes[i], XMVectorSet(pos[j].x, pos[j].y, pos[j].z, 1.0f))) >= 0.0f)
			{
				next = true;
				break;
			}
		}
		if (next)
		{
			continue;
		}
		return false;
	}

	return true;
}
