#include "pch.h"
#include "CameraController.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "SceneManager.h"
#include "PostProcess.h"

TLGameEngine::CameraController::CameraController()
	: Component(Type::Action)
{
}

TLGameEngine::CameraController::~CameraController()
{
	GameEngine::Instance().OnMouseMoveEvent = nullptr;
}

void TLGameEngine::CameraController::Awake()
{
	m_transform = GetTransform();
	m_input = &Input::Instance();
	m_camera = m_gameObject.lock()->GetComponent<Camera>();
	m_postProcess = m_gameObject.lock()->GetComponent<PostProcess>();

	GameEngine::Instance().OnMouseMoveEvent = std::bind(&CameraController::OnMouseMove, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

void TLGameEngine::CameraController::Update()
{
	float deltaTime = Time::Instance().GetDeltaTime();
	float speed = deltaTime * m_moveSpeed;

	if (m_input->GetKey(vk_alpha('w')))
	{
		Walk(speed);
	}
	else if (m_input->GetKey(vk_alpha('s')))
	{
		Walk(-speed);
	}
	if (m_input->GetKey(vk_alpha('d')))
	{
		Strafe(speed);
	}
	else if (m_input->GetKey(vk_alpha('a')))
	{
		Strafe(-speed);
	}
	if (m_input->GetKey(vk_alpha('q')))
	{
		UpDown(-speed);
	}
	else if (m_input->GetKey(vk_alpha('e')))
	{
		UpDown(speed);
	}

	static int sceneNum;
	if (m_input->GetKey(vk_alpha('t')))
	{
		SceneManager::Instance().UnloadScene(sceneNum);

		sceneNum++;
		if (sceneNum > 2)
		{
			sceneNum = 0;
		}

		SceneManager::Instance().LoadScene(sceneNum);
	}

	if (m_input->GetKeyDown(vk_num('1')))
	{
		onOffBloom = !onOffBloom;
		m_postProcess.lock()->SetBloom(onOffBloom);
	}

	if (m_input->GetKeyDown(vk_num('2')))
	{
		onOffTone = !onOffTone;
		m_postProcess.lock()->SetToneMap(onOffTone);
	}

	if (m_input->GetKeyDown(vk_num('3')))
	{
		min -= 0.1f;
		if (min < 0.f)
			min = 0.1f;

		m_postProcess.lock()->SetBloomThreshold(min, max);
	}
	else if (m_input->GetKeyDown(vk_num('4')))
	{
		min += 0.1f;
		if (min > 2.f)
			min = 2.f;
		m_postProcess.lock()->SetBloomThreshold(min, max);
	}

	if (m_input->GetKeyDown(vk_num('5')))
	{
		max -= 0.1f;
		if (max > 2.f)
			max = 2.f;
		m_postProcess.lock()->SetBloomThreshold(min, max);
	}
	else if (m_input->GetKeyDown(vk_num('6')))
	{
		max += 0.1f;
		if (max > 2.f)
			max = 2.f;
		m_postProcess.lock()->SetBloomThreshold(min, max);
	}


	if (m_input->GetKeyDown(vk_num('7')))
	{
		exposure--;
		if (exposure < 2)
			exposure = 2;
		m_postProcess.lock()->SetToneMapExposure(exposure);
	}
	else if (m_input->GetKeyDown(vk_num('8')))
	{
		exposure++;
		if (exposure > 5)
			exposure = 5;
		m_postProcess.lock()->SetToneMapExposure(exposure);
	}
}

void TLGameEngine::CameraController::Walk(float value)
{
	m_transform.lock()->Translate(m_transform.lock()->GetForward() * value);
}

void TLGameEngine::CameraController::Strafe(float value)
{
	m_transform.lock()->Translate(m_transform.lock()->GetRight() * value);

}

void TLGameEngine::CameraController::UpDown(float value)
{
	m_transform.lock()->Translate(m_transform.lock()->GetUp() * value);

}

void TLGameEngine::CameraController::Pitch(float angle)
{
	m_transform.lock()->Rotate(angle, 0, 0);
}

void TLGameEngine::CameraController::RotateY(float angle)
{
	m_transform.lock()->Rotate(0, angle, 0);
}

void TLGameEngine::CameraController::OnMouseMove(int buttonState, int x, int y, int middleOfSceenX, int middleOfSceenY)
{
	if ((buttonState & MK_RBUTTON) != 0)
	{
		float delta = Time::Instance().GetDeltaTime();
		float moveX = (x - m_lastMousePosition.x);
		float moveY = (y - m_lastMousePosition.y);

		Pitch(moveY * m_rotationSpeed);
		RotateY(moveX * m_rotationSpeed);
	}
	m_lastMousePosition.x = static_cast<float>(x);
	m_lastMousePosition.y = static_cast<float>(y);
}
