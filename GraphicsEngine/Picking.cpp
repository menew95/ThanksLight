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
	/// 1. ������ ���( "Ax + By + Cz + D = 0")�� �����ϴ� ����(������ ��)�� ã�´�.
	///	2. ������ ���( "Ax + By + Cz + D = 0")�� �����ϴ� ����(����� ��)�� ã�´�.
	/// 3. �����ϴ� ��� ���� ���� ��� �ȿ� �ִ��� üũ

	/// ���� ������ �̿��� ����� �븻�� ���
	/// U = vertex2 - vertex1
	/// V = vertex3 - vertex1
	/// faceNormal = U X V; => �븻������ �ʿ�
	/// A = faceNormal.x
	/// B = faceNormal.y
	/// C = faceNormal.z
	/// D = -(faceNormal.x * vertex1.x) - (faceNormal.y * vertex1.y) - (faceNormal.z * vertex1.z)
	///
	/// t(������ �ﰢ�� ���� �����ϴ� ����(ī�޶�)���κ����� �Ÿ�, �����̸� ī�޶� �ڿ� �ִ�)
	/// = -(A*x2 + B*y2 + C*z2 + D) / (A*(x1-x2) + B*(y1-y2) + C*(z1-z2))
	/// (A*(x1-x2) + B*(y1-y2) + C*(z1-z2)) = 0 �ϰ�� ���� ������ ����(��� ���ʿ�)
	/// x1, y1, z1 ������ ����(ī�޶� ��ġ) x2, y2, z2 ������ ����
	/// 
	/// planePoint(������� ��������) = rayStart(���� ��������) + rayDir(������ ����) * t
	/// 
	/// planePoint�� ����� ���� ���� ������ ������ �̿� 

	bool isPicking = false;

	// ������ NDC �������� ��ȯ
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

	// ���� ���� ������ ���� (NDC ��������)
	XMVECTOR rayOrigin = XMVectorSet(x, y, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// NDC �������� ��ȯ�� 6���� ����� �̷�� ���� �ε��� CCW
	int makeForPlaneIndex[] = {
		0, 3, 2, 1,	// ��
		7, 4, 5, 6,	// ��
		4, 0, 1, 5,	// ����
		3, 7, 6, 2,	// ����
		1, 2, 6, 5,	// ��
		4, 7, 3, 0	// �Ʒ�
	};


	for (int i = 0; i < 6; i++)
	{

		// ����� �븻 ���
		XMVECTOR U = DirectX::XMVectorSubtract(ndc_boundingVertex[makeForPlaneIndex[i * 4 + 1]], ndc_boundingVertex[makeForPlaneIndex[i * 4]]);
		XMVECTOR V = DirectX::XMVectorSubtract(ndc_boundingVertex[makeForPlaneIndex[i * 4 + 2]], ndc_boundingVertex[makeForPlaneIndex[i * 4]]);
		XMVECTOR faceNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(U, V));

		// ����� ������
		float Ax = DirectX::XMVectorGetX(faceNormal) * DirectX::XMVectorGetX(ndc_boundingVertex[makeForPlaneIndex[i * 4]]);
		float By = DirectX::XMVectorGetY(faceNormal) * DirectX::XMVectorGetY(ndc_boundingVertex[makeForPlaneIndex[i * 4]]);
		float Cz = DirectX::XMVectorGetZ(faceNormal) * DirectX::XMVectorGetZ(ndc_boundingVertex[makeForPlaneIndex[i * 4]]);
		float D = -1 * (Ax + By + Cz);

		float ep1 = (DirectX::XMVectorGetX(rayOrigin) * Ax) + (DirectX::XMVectorGetY(rayOrigin) * By) + (DirectX::XMVectorGetZ(rayOrigin) * Cz);
		float ep2 = (DirectX::XMVectorGetX(rayDir) * Ax) + (DirectX::XMVectorGetY(rayDir) * By) + (DirectX::XMVectorGetZ(rayDir) * Cz);

		// ������ �ﰢ�� ���� �����ϴ� ����(ī�޶�)���κ����� �Ÿ�
		float t = -(ep1 + D) / ep2;

		// planeIntersect(��� ���� �������� ��������)
		XMVECTOR planeIntersect = DirectX::XMVectorAdd(rayOrigin, DirectX::XMVectorScale(rayDir, t));

		// ���������� ���ȿ� �ִ��� üũ
		XMVECTOR triV1 = ndc_boundingVertex[makeForPlaneIndex[i * 4]];
		XMVECTOR triV2 = ndc_boundingVertex[makeForPlaneIndex[i * 4 + 1]];
		XMVECTOR triV3 = ndc_boundingVertex[makeForPlaneIndex[i * 4 + 2]];
		XMVECTOR triV4 = ndc_boundingVertex[makeForPlaneIndex[i * 4 + 3]];

		// �� ũ�ν����� ������ �ٸ��� ���� => �������� ������ �Ǵ� ����(����� �̷��)�� �ۿ� �ִ�
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
						// �װ��� ���� �ȿ� �ִ� => �������� ���� �װ��� �̷���� ����� ���ʿ� �ִ�.
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
