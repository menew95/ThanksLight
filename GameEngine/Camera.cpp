#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "GameEngine.h"
#include "IGraphicsEngine.h"

std::weak_ptr<TLGameEngine::Camera> TLGameEngine::Camera::m_mainCamera;

TLGameEngine::Camera::Camera()
	: Component(Type::Camera)
{
}

TLGameEngine::Camera::~Camera()
{
	if (m_mainCamera.expired())
	{
		m_mainCamera.reset();
	}
}

void TLGameEngine::Camera::Awake()
{
	if (m_mainCamera.lock() == nullptr)
	{
		m_mainCamera = shared_from_this();
	}
}

void TLGameEngine::Camera::LateUpdate()
{
	// 직교 보정 구현 필요함.
	std::shared_ptr<Transform> transform = GetTransform();

	m_viewMatrix = transform->GetWorldTM().Invert();


	DirectX::SimpleMath::Vector3 pos = transform->GetWorldPosition();
	DirectX::SimpleMath::Vector3 lookAt = transform->GetForward();
	DirectX::SimpleMath::Vector3 rot = transform->GetWorldRotate();
	DirectX::SimpleMath::Vector3 up = transform->GetUp();
	DirectX::SimpleMath::Vector3 right = transform->GetRight();

	float deg2Rad = 3.14159265359f * 2 / 360.f;
	GameEngine::Instance().GetGraphicsEngine()->SetLens(field_of_view * deg2Rad, m_near, m_far);
	GameEngine::Instance().GetGraphicsEngine()->SetCameraPosition(pos.x, pos.y, pos.z);
	GameEngine::Instance().GetGraphicsEngine()->SetCameraLookAt(lookAt.x, lookAt.y, lookAt.z);
	GameEngine::Instance().GetGraphicsEngine()->SetCameraRight(right.x, right.y, right.z);
	GameEngine::Instance().GetGraphicsEngine()->FrustumUpdate(
		pos.x, pos.y, pos.z,
		lookAt.x, lookAt.y, lookAt.z,
		up.x, up.y, up.z,
		field_of_view * 2, m_aspectRadio, m_near, m_far);
}

