#pragma once

/// <summary>
/// 엔진 카메라
/// </summary>
namespace TLGraphicsEngine
{
	class EngineCamera
	{
	public:
		EngineCamera();
		~EngineCamera();

	public:
		void OnResize(float aspectRatio);

		void LookAt(DirectX::XMVECTOR camPos, DirectX::XMVECTOR targetPos, DirectX::XMVECTOR upDir);
		void LookAt(float posX, float posY, float posZ, float posW,
					float targetX, float targetY, float targetZ, float targetW,
					float worldUpX, float worldUpY, float worldUpZ, float worldUpW);
		void LookAt(float eyeX, float eyeY, float eyeZ,
			float focX, float focY, float focZ);

		void SetLens(float fovY, float aspect, float zn, float zf);
		void SetLens(float fovY, float zn, float zf);
		void UpdateView();

	public:
		XMFLOAT3 GetLookAt() { return m_LookAt; }
		void SetLookAt(float x, float y, float z) { m_LookAt = XMFLOAT3(x, y, z); }
		XMFLOAT3 GetRight() { return m_Right; }
		void SetRight(float x, float y, float z) { m_Right = XMFLOAT3(x, y, z); }
		XMFLOAT3 GetUp() { return m_Up; }
		void SetUp(float x, float y, float z) { m_Up = XMFLOAT3(x, y, z); }
		XMFLOAT3 GetPosition() { return m_Position; }
		void SetPosition(float x, float y, float z) { m_Position = XMFLOAT3(x, y, z); }

		DirectX::XMMATRIX GetProjTM();
		DirectX::XMMATRIX GetViewTM();
		DirectX::XMMATRIX GetViewProjTM();

		float GetNear() { return m_Near; }
		float GetFar() { return m_Far; }
		float GetAspect() { return m_Aspect; }
		float GetFovY() { return m_FovY; }
		float GetNearWindowHeight() { return m_NearWindowHeight; }
		float GetWindowHeight() { return m_FarWindowHeight; }

	private:
		DirectX::XMFLOAT3 m_LookAt;
		DirectX::XMFLOAT3 m_Right;
		DirectX::XMFLOAT3 m_Up;
		DirectX::XMFLOAT3 m_Position;

		DirectX::XMFLOAT4X4 m_WorldMatrix;
		DirectX::XMFLOAT4X4 m_ViewMatrix;
		DirectX::XMFLOAT4X4 m_ProjMatrix;

		float m_Near;;
		float m_Far;
		float m_Aspect;
		float m_FovY;
		float m_NearWindowHeight;
		float m_FarWindowHeight;
	};
}

