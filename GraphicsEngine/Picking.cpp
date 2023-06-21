#include "pch.h"
#include "Picking.h"
#include "EngineCamera.h"
#include "Mesh.h"

using namespace TLGraphicsEngine;

Picking::~Picking()
{

}

Picking::Picking()
{

}

bool Picking::CheckPicking(Mesh* mesh, DirectX::XMMATRIX meshWorld, float x, float y, EngineCamera* engineCamera)
{
	/// 1. 광선과 평면( "Ax + By + Cz + D = 0")이 교차하는 지점(광선의 점)을 찾는다.
	///	2. 광선과 평면( "Ax + By + Cz + D = 0")이 교차하는 지점(평면의 점)을 찾는다.
	/// 3. 교차하는 평면 위의 점이 평면 안에 있는지 체크

	/// 정점 세개를 이용해 평면의 노말값 계산
	/// U = vertex2 - vertex1
	/// V = vertex3 - vertex1
	/// faceNormal = U X V; => 노말라이즈 필요
	/// A = faceNormal.x
	/// B = faceNormal.y
	/// C = faceNormal.z
	/// D = -(faceNormal.x * vertex1.x) - (faceNormal.y * vertex1.y) - (faceNormal.z * vertex1.z)
	///
	/// t(광선이 삼각형 평면과 교차하는 광선(카메라)으로부터의 거리, 음수이면 카메라 뒤에 있다)
	/// = -(A*x2 + B*y2 + C*z2 + D) / (A*(x1-x2) + B*(y1-y2) + C*(z1-z2))
	/// (A*(x1-x2) + B*(y1-y2) + C*(z1-z2)) = 0 일경우 평면과 직선이 평행(계산 불필요)
	/// x1, y1, z1 광선의 원점(카메라 위치) x2, y2, z2 광선의 방향
	/// 
	/// planePoint(평면위의 교차지점) = rayStart(광선 시작지점) + rayDir(광선의 방향) * t
	/// 
	/// planePoint와 평면의 구성 정점 사이의 직선을 이용 

	bool isPicking = false;

	// 정점을 NDC 공간으로 변환
	XMVECTOR ndc_boundingVertex[8];
	//for (int i = 0; i < 8; i++)
	//{
	//	XMVECTOR ndcPos = DirectX::XMLoadFloat3(&boundingVertex[i]);
	//	ndc_boundingVertex[i] = DirectX::XMLoadFloat3(&boundingVertex[i]);
	//	//ndc_boundingVertex[i] = DirectX::XMVector3Transform(ndcPos, m_EngineCamera->GetProjTM());
	//}
	XMVECTOR minVec = DirectX::XMVectorSet(-1, -1, 0, 0);
	XMVECTOR maxVec = DirectX::XMVectorSet(+1, +1, 1, 0);
	for (int i = 0; i < 8; i++)
	{
		XMVECTOR v1 = DirectX::XMLoadFloat3(&mesh->m_BoundingBoxVertices[i].Pos);
		ndc_boundingVertex[i] = DirectX::XMVector3Transform(v1, meshWorld * engineCamera->GetViewProjTM());
		ndc_boundingVertex[i] = DirectX::XMVectorScale(ndc_boundingVertex[i], 1 / DirectX::XMVectorGetW(ndc_boundingVertex[i]));

		ndc_boundingVertex[i] = DirectX::XMVectorClamp(ndc_boundingVertex[i], minVec, maxVec);
	}

	// 레이 시작 지점과 방향 (NDC 공간상의)
	XMVECTOR rayOrigin = XMVectorSet(x, y, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// NDC 공간으로 변환한 6개의 평면을 이루는 정점 인덱스 CCW
	int makeForPlaneIndex[] = {
		0, 3, 2, 1,	// 앞
		7, 4, 5, 6,	// 뒤
		4, 0, 1, 5,	// 좌측
		3, 7, 6, 2,	// 우측
		1, 2, 6, 5,	// 위
		4, 7, 3, 0	// 아래
	};


	for (int i = 0; i < 6; i++)
	{

		// 평면의 노말 계산
		XMVECTOR U = DirectX::XMVectorSubtract(ndc_boundingVertex[makeForPlaneIndex[i * 4 + 1]], ndc_boundingVertex[makeForPlaneIndex[i * 4]]);
		XMVECTOR V = DirectX::XMVectorSubtract(ndc_boundingVertex[makeForPlaneIndex[i * 4 + 2]], ndc_boundingVertex[makeForPlaneIndex[i * 4]]);
		XMVECTOR faceNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(U, V));

		// 평면의 방정식
		float Ax = DirectX::XMVectorGetX(faceNormal) * DirectX::XMVectorGetX(ndc_boundingVertex[makeForPlaneIndex[i * 4]]);
		float By = DirectX::XMVectorGetY(faceNormal) * DirectX::XMVectorGetY(ndc_boundingVertex[makeForPlaneIndex[i * 4]]);
		float Cz = DirectX::XMVectorGetZ(faceNormal) * DirectX::XMVectorGetZ(ndc_boundingVertex[makeForPlaneIndex[i * 4]]);
		float D = -1 * (Ax + By + Cz);

		float ep1 = (DirectX::XMVectorGetX(rayOrigin) * Ax) + (DirectX::XMVectorGetY(rayOrigin) * By) + (DirectX::XMVectorGetZ(rayOrigin) * Cz);
		float ep2 = (DirectX::XMVectorGetX(rayDir) * Ax) + (DirectX::XMVectorGetY(rayDir) * By) + (DirectX::XMVectorGetZ(rayDir) * Cz);

		// 광선이 삼각형 평면과 교차하는 광선(카메라)으로부터의 거리
		float t = -(ep1 + D) / ep2;

		// planeIntersect(평면 위의 광선과의 교차지점)
		XMVECTOR planeIntersect = DirectX::XMVectorAdd(rayOrigin, DirectX::XMVectorScale(rayDir, t));

		// 교차지점이 평면안에 있는지 체크
		XMVECTOR triV1 = ndc_boundingVertex[makeForPlaneIndex[i * 4]];
		XMVECTOR triV2 = ndc_boundingVertex[makeForPlaneIndex[i * 4 + 1]];
		XMVECTOR triV3 = ndc_boundingVertex[makeForPlaneIndex[i * 4 + 2]];
		XMVECTOR triV4 = ndc_boundingVertex[makeForPlaneIndex[i * 4 + 3]];

		// 두 크로스곱의 방향이 다르면 음수 => 교차점이 기준이 되는 직선(평면을 이루는)의 밖에 있다
		XMVECTOR cp1 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triV2, triV1), DirectX::XMVectorSubtract(planeIntersect, triV1));
		XMVECTOR cp2 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triV2, triV1), DirectX::XMVectorSubtract(triV4, triV1));
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(cp1, cp2)) >= 0)
		{
			cp1 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triV3, triV2), DirectX::XMVectorSubtract(planeIntersect, triV2));
			cp2 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triV3, triV2), DirectX::XMVectorSubtract(triV1, triV2));
			if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(cp1, cp2)) >= 0)
			{
				cp1 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triV4, triV3), DirectX::XMVectorSubtract(planeIntersect, triV3));
				cp2 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triV4, triV3), DirectX::XMVectorSubtract(triV2, triV3));
				if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(cp1, cp2)) >= 0)
				{
					cp1 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triV1, triV4), DirectX::XMVectorSubtract(planeIntersect, triV4));
					cp2 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triV1, triV4), DirectX::XMVectorSubtract(triV3, triV4));
					if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(cp1, cp2)) >= 0)
					{
						// 네개의 직선 안에 있다 => 교차점이 정점 네개로 이루어진 평면의 안쪽에 있다.
						isPicking = true;
						break;
					}
				}
			}
		}
	}


	mesh->m_IsPicking = isPicking;

	return isPicking;
}
