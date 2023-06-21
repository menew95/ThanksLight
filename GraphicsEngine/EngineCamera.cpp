#include "pch.h"
#include "EngineCamera.h"

using namespace TLGraphicsEngine;

EngineCamera::EngineCamera() :
	m_Near(0.0001f), m_Far(1000.0f), m_Aspect(0), m_FovY(0.5f * MathHelper::Pi), m_NearWindowHeight(0), m_FarWindowHeight(0),
	m_LookAt(0.0f, 0.0f, 1.0f), m_Right(1.0f, 0.0f, 0.0f), m_Up(0.0f, 1.0f, 0.0f), m_Position(0.0f, 0.0f, 0.0f)
{
	DirectX::XMMATRIX i = DirectX::XMMatrixIdentity();

	DirectX::XMStoreFloat4x4(&m_WorldMatrix, i);
	DirectX::XMStoreFloat4x4(&m_ViewMatrix, i);
	DirectX::XMStoreFloat4x4(&m_ProjMatrix, i);
}

EngineCamera::~EngineCamera()
{

}

void EngineCamera::UpdateView()
{
	/*DirectX::XMVECTOR eye, foc, up, right;

	eye = DirectX::XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 0.f);
	foc = DirectX::XMVectorSet(m_LookAt.x, m_LookAt.y, m_LookAt.z, 0.f);
	up = DirectX::XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.f);

	right = DirectX::XMVector3Cross(up, foc);

	right = DirectX::XMVector3Normalize(right);

	DirectX::XMMATRIX m = DirectX::XMMatrixLookAtLH(eye, DirectX::XMVectorAdd(eye,foc), up);

	DirectX::XMStoreFloat4x4(&m_ViewMatrix, m);*/

	DirectX::XMVECTOR P, L, U, R;

	P = DirectX::XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 0.f);
	L = DirectX::XMVectorSet(m_LookAt.x, m_LookAt.y, m_LookAt.z, 0.f);
	U = DirectX::XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.f);
	R = DirectX::XMVectorSet(m_Right.x, m_Right.y, m_Right.z, 0.f);

	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));
	R = XMVector3Cross(U, L);

	DirectX::XMMATRIX m = DirectX::XMMatrixLookAtLH(P, DirectX::XMVectorAdd(P, L), U);

	m_ViewMatrix._11 = XMVectorGetX(R);	m_ViewMatrix._12 = XMVectorGetX(U);	m_ViewMatrix._13 = XMVectorGetX(L);	m_ViewMatrix._14 = 0;
	m_ViewMatrix._21 = XMVectorGetY(R);	m_ViewMatrix._22 = XMVectorGetY(U);	m_ViewMatrix._23 = XMVectorGetY(L);	m_ViewMatrix._24 = 0;
	m_ViewMatrix._31 = XMVectorGetZ(R);	m_ViewMatrix._32 = XMVectorGetZ(U);	m_ViewMatrix._33 = XMVectorGetZ(L);	m_ViewMatrix._34 = 0;

	float x = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

	m_ViewMatrix._41 = -x;	m_ViewMatrix._42 = -y;	m_ViewMatrix._43 = -z;	m_ViewMatrix._44 = 1;
}

void EngineCamera::LookAt(DirectX::XMVECTOR camPos, DirectX::XMVECTOR targetPos, DirectX::XMVECTOR upDir)
{
	DirectX::XMVECTOR L = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetPos, camPos));
	DirectX::XMVECTOR R = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upDir, L));
	DirectX::XMVECTOR U = DirectX::XMVector3Cross(L, R);

	DirectX::XMStoreFloat3(&m_Position, camPos);
	DirectX::XMStoreFloat3(&m_LookAt, L);
	DirectX::XMStoreFloat3(&m_Right, R);
	DirectX::XMStoreFloat3(&m_Up, U);
}

void EngineCamera::LookAt(float posX, float posY, float posZ, float posW, float targetX, float targetY, float targetZ, float targetW, float worldUpX, float worldUpY, float worldUpZ, float worldUpW)
{
	DirectX::XMVECTOR camPos, targetPos, upDir;

	m_Position.x = posX;
	m_Position.y = posY;
	m_Position.z = posZ;

	camPos = DirectX::XMVectorSet(posX, posY, posZ, 0.f);
	targetPos = DirectX::XMVectorSet(targetX, targetY, targetZ, 0.f);
	upDir = DirectX::XMVectorSet(worldUpX, worldUpY, worldUpZ, 0.f);

	DirectX::XMVECTOR L = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetPos, camPos));
	DirectX::XMVECTOR R = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upDir, L));
	DirectX::XMVECTOR U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));

	DirectX::XMStoreFloat3(&m_LookAt, L);
	DirectX::XMStoreFloat3(&m_Right, R);
	DirectX::XMStoreFloat3(&m_Up, U);
}

void EngineCamera::LookAt(float eyeX, float eyeY, float eyeZ, float focX, float focY, float focZ)
{
	m_Position.x = eyeX;
	m_Position.y = eyeY;
	m_Position.z = eyeZ;

	m_LookAt.x = focX;
	m_LookAt.y = focY;
	m_LookAt.z = focZ;
}

DirectX::XMMATRIX EngineCamera::GetViewTM()
{
	return  DirectX::XMMatrixSet(
		m_ViewMatrix._11, m_ViewMatrix._12, m_ViewMatrix._13, m_ViewMatrix._14,
		m_ViewMatrix._21, m_ViewMatrix._22, m_ViewMatrix._23, m_ViewMatrix._24,
		m_ViewMatrix._31, m_ViewMatrix._32, m_ViewMatrix._33, m_ViewMatrix._34,
		m_ViewMatrix._41, m_ViewMatrix._42, m_ViewMatrix._43, m_ViewMatrix._44
	);
}

DirectX::XMMATRIX EngineCamera::GetProjTM()
{
	return DirectX::XMMatrixSet(
		m_ProjMatrix._11, m_ProjMatrix._12, m_ProjMatrix._13, m_ProjMatrix._14,
		m_ProjMatrix._21, m_ProjMatrix._22, m_ProjMatrix._23, m_ProjMatrix._24,
		m_ProjMatrix._31, m_ProjMatrix._32, m_ProjMatrix._33, m_ProjMatrix._34,
		m_ProjMatrix._41, m_ProjMatrix._42, m_ProjMatrix._43, m_ProjMatrix._44
	);
}

void EngineCamera::OnResize(float aspectRatio)
{
	m_Aspect = aspectRatio;
	DirectX::XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(m_FovY, aspectRatio, m_Near, m_Far);
	DirectX::XMStoreFloat4x4(&m_ProjMatrix, p);
}

DirectX::XMMATRIX EngineCamera::GetViewProjTM()
{
	return  DirectX::XMMatrixMultiply(GetViewTM(), GetProjTM());
}

void EngineCamera::SetLens(float fovY, float zn, float zf)
{
	m_FovY = fovY;
	m_Near = zn;
	m_Far = zf;

	m_NearWindowHeight = 2.0f * m_Near * tanf(0.5f * m_FovY);
	m_FarWindowHeight = 2.0f * m_Far * tanf(0.5f * m_FovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_Near, m_Far);
	XMStoreFloat4x4(&m_ProjMatrix, P);
}

void EngineCamera::SetLens(float fovY, float aspect, float zn, float zf)
{
	m_FovY = fovY;
	m_Aspect = aspect;
	m_Near = zn;
	m_Far = zf;

	m_NearWindowHeight = 2.0f * m_Near * tanf(0.5f * m_FovY);
	m_FarWindowHeight = 2.0f * m_Far * tanf(0.5f * m_FovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_Near, m_Far);
	XMStoreFloat4x4(&m_ProjMatrix, P);
}
