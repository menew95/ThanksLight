#pragma once
#include "DX11Define.h"

namespace TLGraphicsEngine
{
	class Mesh;
	class EngineCamera;

	/// <summary>
	/// View Frustum Culling Class
	/// </summary>
	class Frustum
	{
	public:
		Frustum(EngineCamera* engineCamera);
		virtual ~Frustum();

	public:
		void FrustumUpdate(float posx, float posy, float posz, float forwardx,
			float forwardy, float forwardz, float upx, float upy, float upz,
			float fovY, float aspectRatio, float nearZ, float farZ);
		void Update(float, XMMATRIX, XMMATRIX);
		XMFLOAT3* Setting(XMFLOAT3 (&boundingVertex)[8], Mesh* mesh/*, Mesh* debugMesh*/, XMMATRIX meshWorld);

		bool CheckPoint(float, float, float);
		bool CheckCube(float, float, float, float);
		bool CheckSphere(float, float, float, float);
		bool CheckRectangle(float, float, float, float, float, float);
		bool CheckBoundingBox(XMFLOAT3 pos[8]);

	private:
		EngineCamera* m_EngineCamera;
		XMVECTOR		m_Planes[6];
		XMMATRIX		m_TestFrustumCamWorld;
		XMMATRIX		m_TestFrustumView;
		XMMATRIX		m_TestFrustumProj;
	};
}

