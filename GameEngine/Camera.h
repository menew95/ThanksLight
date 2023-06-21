#pragma once

#include "Component.h"

namespace TLGameEngine
{
class Camera : public Component, public std::enable_shared_from_this<Camera>
{
public:
	Camera();
	virtual ~Camera();

	virtual void Awake() override;
	virtual void LateUpdate() override;

	Matrix GetViewMatrix() { return m_viewMatrix; }
	Matrix GetProjectionMatrix() { return m_projectionMatrix; }

	static std::shared_ptr<Camera> GetMainCamera() { return m_mainCamera.lock(); }
private:
	float m_near = 0.1f;
	float m_far = 1000.f;
	float field_of_view = 90;

	Matrix m_viewMatrix = {};
	Matrix m_projectionMatrix = {};

	float m_aspectRadio = 1.f;

	static std::weak_ptr<Camera> m_mainCamera;

	BOOST_DESCRIBE_CLASS(TLGameEngine::Camera, (), (), (), (	))
};
}

