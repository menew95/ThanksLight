#pragma once

#include "Component.h"

namespace TLGameEngine
{
class CameraController : public Component
{
public:
	CameraController();
	~CameraController();

	virtual void Awake() override;
	virtual void Update() override;

	void Walk(float value);
	void Strafe(float value);
	void UpDown(float value);

	void Pitch(float angle);
	void RotateY(float angle);
private:
	void OnMouseMove(int buttonState, int x, int y, int middleOfSceenX, int middleOfSceenY);

	float m_moveSpeed = 10;
	float m_rotationSpeed = 0.01f;
	Input* m_input;
	std::weak_ptr<Transform> m_transform;
	std::weak_ptr<class Camera> m_camera;

	std::weak_ptr<class PostProcess> m_postProcess;

	Vector2 m_lastMousePosition = Vector2::Zero;

	bool onOffBloom = false;
	bool onOffTone = false;
	float min = 1.0f;
	float max = 1.26f;
	float exposure = 2.0f;

	BOOST_DESCRIBE_CLASS(TLGameEngine::CameraController, (), (), (), ())
};
}

