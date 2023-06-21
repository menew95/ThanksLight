#include "pch.h"
#include "PlayerController.h"

#include "GameObject.h"
#include "Transform.h"
#include "InteractiveObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "CapsuleCollider.h"
#include "Light.h"
#include "GameEngine.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ProjectorObject.h"
#include "FlashLight.h"
#include "GameManager.h"

#include "OutLine.h"
#include "Text.h"
#include "Canvas.h"
#include <iostream>

TLGameEngine::PlayerController::PlayerController() :
	Component(Component::Type::Action),
	m_toggleRotateGrabObject(false),
	m_transTimer(0.f)
{

}

TLGameEngine::PlayerController::~PlayerController()
{
	GameEngine::Instance().OnMouseMoveEvent = nullptr;
}

void TLGameEngine::PlayerController::Awake()
{
	m_cameraTransform = SceneManager::Instance().FindObject(m_cameraID)->GetComponent<Transform>();
	m_gameObject.lock()->GetComponent<CapsuleCollider>()->SetColliderLayer("PLAYER");

	m_rigidBody = m_gameObject.lock()->GetComponent<Rigidbody>();

	m_rigidBody.lock()->SetPhysicsAxisLock(false, false, false, true, true, true);
	m_Transform = GetTransform();

	m_flashLight = m_Transform.lock()->GetChild(0)->GetComponent<FlashLight>();

	GameEngine::Instance().OnMouseMoveEvent = std::bind(&PlayerController::OnMouseMove, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

	m_LButton = SceneManager::Instance().FindObject(m_LButtonID)->GetComponent<Text>();
	m_RButton = SceneManager::Instance().FindObject(m_RButtonID)->GetComponent<Text>();
	m_EButton = SceneManager::Instance().FindObject(m_EButtonID)->GetComponent<Text>();
	m_LButton.lock()->SetFontSize(20);
	m_RButton.lock()->SetFontSize(20);
	m_EButton.lock()->SetFontSize(20);
	ActiveUI(3);
}

void TLGameEngine::PlayerController::OnEnable()
{

}

void TLGameEngine::PlayerController::OnDisalbe()
{

}

void TLGameEngine::PlayerController::Start()
{
	CURSORINFO cursorInfo = { 0 };
	cursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursorInfo);
	if (cursorInfo.flags == CURSOR_SHOWING)
	{
		ShowCursor(false);
	}
}

void TLGameEngine::PlayerController::Update()
{
	if (Input::Instance().GetKeyDown(VK_ESCAPE))
	{
		SceneManager::Instance().UnloadScene(SceneManager::Instance().GetCurrentSceneIndex());
		SceneManager::Instance().LoadScene(0);
	}

	CheckInputs();
}

void TLGameEngine::PlayerController::LateUpdate()
{

}

void TLGameEngine::PlayerController::FixedUpdate()
{

}

void TLGameEngine::PlayerController::OnDestroy()
{
}

void TLGameEngine::PlayerController::SetCamera(std::shared_ptr<TLGameEngine::GameObject> camera)
{
	m_cameraTransform = camera->GetComponent<Transform>();
}

void TLGameEngine::PlayerController::CheckInputs()
{
	RayCastFromCamera();
	if (GameManager::Instance().GetKeybordInput())
	{
		MoveCharacter();
		GrabObject();
		RotateObject();
		TurnOnFlashLight();
	}

}

void TLGameEngine::PlayerController::MoveCharacter()
{
	if (!GameManager::Instance().GetKeybordInput())
	{
		return;
	}

	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::Zero;
	auto transform = GetTransform();
	auto worldPos = transform->GetWorldPosition();

	const std::vector<std::string> filterLayer = { "PLAYER" };
	auto ret = TLGameEngine::PhysicsManager::Instance().RayCast({ worldPos.x, worldPos.y, worldPos.z }, { 0.F, -1.f, 0.f }, 50, filterLayer, PhysicsEngine::ePhysicsType::INVAILD);

	bool isGround = (!ret.m_objName.empty()) && (ret.m_disance <= 1.2f);

	if (TLGameEngine::Input::Instance().GetKeyDown(VK_SPACE) && isGround)
	{
		m_rigidBody.lock()->AddImpulse({ 0, 7.f, 0 });
	}

	if (isGround)
	{
		if (TLGameEngine::Input::Instance().GetKey('W'))
			dir += transform->GetForward();

		if (TLGameEngine::Input::Instance().GetKey('S'))
			dir -= transform->GetForward();

		if (TLGameEngine::Input::Instance().GetKey('A'))
			dir -= transform->GetRight();

		if (TLGameEngine::Input::Instance().GetKey('D'))
			dir += transform->GetRight();

		dir.Normalize();
		dir *= 17.f;
	}
	else
	{
		if (TLGameEngine::Input::Instance().GetKey('W'))
			dir += transform->GetForward();

		if (TLGameEngine::Input::Instance().GetKey('S'))
			dir -= transform->GetForward();

		if (TLGameEngine::Input::Instance().GetKey('A'))
			dir -= transform->GetRight();

		if (TLGameEngine::Input::Instance().GetKey('D'))
			dir += transform->GetRight();

		dir.Normalize();
		dir *= 7.f;
	}

	m_rigidBody.lock()->AddForce({ dir.x, dir.y, dir.z });
}

void TLGameEngine::PlayerController::GrabObject()
{
	bool isGrabObjectNull = (m_grabObject.lock() == nullptr);

	if (TLGameEngine::Input::Instance().GetKeyDown('E'))
	{
		if (isGrabObjectNull)
		{
			//그랩한 오브젝트가 없을때
			if (m_rayHitObject.lock())
			{
				auto interactiveObjectComp = m_rayHitObject.lock()->GetComponent<InteractiveObject>();
				if (interactiveObjectComp != nullptr)
				{
					if (!interactiveObjectComp->GetIsNotGrabbable())
					{
						m_grabObject = m_rayHitObject;
						interactiveObjectComp->GrabObject(m_Transform.lock());
						if (!interactiveObjectComp->GetIs2DObject())
						{
							SnapGrabObject();
						}
					}
				}
			}
		}
		else
		{
			//그랩한 오브젝트가 있을떄
			auto interactiveObjectComp = m_grabObject.lock()->GetComponent<InteractiveObject>();
			interactiveObjectComp->DropObject();

			m_grabObject.reset();
			m_toggleRotateGrabObject = false;
		}
	}

	isGrabObjectNull = (m_grabObject.lock() == nullptr);

	if (!isGrabObjectNull)
	{
		DirectX::SimpleMath::Vector4 holdPosition = DirectX::SimpleMath::Vector4::Transform(m_holdPosition, m_cameraTransform.lock()->GetWorldTM());
		auto grabObjectPos = m_grabObject.lock()->GetComponent<Transform>()->GetWorldPosition();
		DirectX::SimpleMath::Vector3 forceDir = holdPosition - grabObjectPos;

		auto interactiveObjectComp = m_grabObject.lock()->GetComponent<InteractiveObject>();

		if (forceDir.Length() > 10.f)
		{
			if (interactiveObjectComp != nullptr)
			{
				interactiveObjectComp->DropObject();
			}

			m_grabObject.reset();
			m_toggleRotateGrabObject = false;
		}
		else if (forceDir.Length() < 0.1f)
		{

			interactiveObjectComp->MoveToHoldPosition({0.f, 0.f, 0.f});
		}
		else
		{
			forceDir.Normalize();
			forceDir *= 5.f;

			interactiveObjectComp->MoveToHoldPosition(forceDir);
		}
	}

}

void TLGameEngine::PlayerController::OnMouseMove(int buttonState, int x, int y, int middleOfSceenX, int middleOfSceenY)
{
	if (m_isHoldMouse)
	{
		SetCursorPos(middleOfSceenX, middleOfSceenY);
	}
	float delta = Time::Instance().GetDeltaTime();
	float moveX = (x - middleOfSceenX);
	float moveY = (y - middleOfSceenY);

	if (m_toggleRotateGrabObject == false)
	{
		float yaw = 0.001f * moveX;
		float pitch = 0.001f * moveY;

		DirectX::SimpleMath::Vector3 moveVector = DirectX::SimpleMath::Vector3::Zero;

		moveVector.x = pitch;
		moveVector.y = 0;
		moveVector.z = 0;

		m_cameraTransform.lock()->Rotate(moveVector);

		moveVector.x = 0;
		moveVector.y = yaw;
		moveVector.z = 0;
		GetTransform()->Rotate(moveVector);

		moveVector.x = pitch;
		if (m_grabObject.lock())
		{
			MaintainGrabObjectRotation();
		}
	}
	else
	{
		float yaw = 0.01f * moveX;
		float pitch = 0.01f * moveY;

		DirectX::SimpleMath::Vector2 moveVector = DirectX::SimpleMath::Vector2::Zero;

		moveVector.x = pitch;
		moveVector.y = yaw;

		if (TLGameEngine::Input::Instance().GetKey(VK_MBUTTON))
		{
			RotateGrabObject(moveVector);
		}
	}
}

void TLGameEngine::PlayerController::RotateObject()
{
	if (TLGameEngine::Input::Instance().GetKey(VK_MBUTTON) && (m_grabObject.lock()))
	{
		m_toggleRotateGrabObject = true;
	}

	if (TLGameEngine::Input::Instance().GetKeyUp(VK_MBUTTON) && (m_grabObject.lock()))
	{
		if (GameManager::Instance().GetKeybordInput())
		{
			SnapGrabObject();
		}
		m_toggleRotateGrabObject = false;
	}
}

void TLGameEngine::PlayerController::TurnOnFlashLight()
{
	if (TLGameEngine::Input::Instance().GetKey(VK_LBUTTON))
	{
		bool isConsist = false;

		// 라이트를 켜주고 그래픽스에 커스텀 버퍼를 설정 해준다.
		if (!m_flashLight.expired())
		{
			m_flashLight.lock()->OnOffLight(true);
		}

		if (m_rayHitObject.lock() && m_previousRayHitObject.lock())
		{
			if (m_rayHitObject.lock()->GetTag() == "ProjectedObject")
			{
				auto _projectedObject = m_rayHitObject.lock()->GetComponent<ProjectorObject>();
				if (_projectedObject != nullptr)
				{
					_projectedObject->Set2D();
				}
				else
				{
					assert(false);
				}
			}
			else
			{
				auto isInteractiveObject = m_rayHitObject.lock()->GetComponent<InteractiveObject>();

				if (isInteractiveObject != nullptr)
				{
					if (!isInteractiveObject->GetIsConvert())
					{
						if (isInteractiveObject->GetIs2DObject())
						{
							if (m_previousRayHitObject.lock()->GetUID() == m_rayHitObject.lock()->GetUID())
							{
								const static int changeTime = 0.5f;
								m_transTimer += TLGameEngine::Time::Instance().GetDeltaTime();
								isConsist = true;

								if (m_transTimer > changeTime)
								{
									isInteractiveObject->Set3DObject();
								}
							}
						}
					}
				}
			}
		}

		if (!isConsist)
		{
			m_transTimer = 0.0f;
		}
	}
	else
	{
		if (!m_flashLight.expired())
		{
			m_flashLight.lock()->OnOffLight(false);
		}

		m_transTimer = 0.0f;
	}

	if (TLGameEngine::Input::Instance().GetKey(VK_RBUTTON) && (m_grabObject.lock()))
	{
		auto interactiveObjectComp = m_grabObject.lock()->GetComponent<InteractiveObject>();

		if (interactiveObjectComp != nullptr)
		{
			if (!interactiveObjectComp->GetIsConvert())
			{
				interactiveObjectComp->DropObject();
				interactiveObjectComp->Set2DObject();

				m_grabObject.reset();
				m_toggleRotateGrabObject = false;
			}
		}
	}
}


void TLGameEngine::PlayerController::MaintainGrabObjectRotation()
{

	auto isInteractiveObject = m_grabObject.lock()->GetComponent<InteractiveObject>();

	if (isInteractiveObject != nullptr)
	{
		auto CameraRot = m_cameraTransform.lock()->GetWorldTM();
		isInteractiveObject->SetAxisRotation(CameraRot);
	}
}

void TLGameEngine::PlayerController::RotateGrabObject(Vector2 mousePos)
{
	DirectX::SimpleMath::Vector3 moveVector = DirectX::SimpleMath::Vector3::Zero;

	auto grabObjectTransform = m_grabObject.lock()->GetComponent<Transform>();
	auto grabObjWorldTM = grabObjectTransform->GetWorldTM();
	auto CameraWorldTM = m_cameraTransform.lock()->GetWorldTM();
	auto CameraWorldTMInverse = CameraWorldTM.Invert();

	auto grabObjLocalTM = grabObjWorldTM * CameraWorldTMInverse;

	DirectX::SimpleMath::Vector3 localscl;
	DirectX::SimpleMath::Quaternion localrot;
	DirectX::SimpleMath::Vector3 localpos;

	grabObjLocalTM.Decompose(localscl, localrot, localpos);

	moveVector.x = mousePos.x;
	moveVector.y = mousePos.y;
	moveVector.z = 0;

	auto addRot = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(moveVector);

	localrot *= addRot;

	auto sclTM = DirectX::SimpleMath::Matrix::CreateScale(localscl);
	auto rotTM = DirectX::SimpleMath::Matrix::CreateFromQuaternion(localrot);
	auto posTM = DirectX::SimpleMath::Matrix::CreateTranslation(localpos);

	grabObjLocalTM = sclTM * rotTM * posTM;
	grabObjWorldTM = grabObjLocalTM * CameraWorldTM;

	grabObjWorldTM.Decompose(localscl, localrot, localpos);

	grabObjectTransform->SetWorldRotateQ(localrot);
}

void TLGameEngine::PlayerController::SnapGrabObject()
{
	auto isInteractiveObj = m_grabObject.lock()->GetComponent<InteractiveObject>();
	if (isInteractiveObj != nullptr)
	{
		auto CameraRot = m_cameraTransform.lock()->GetWorldTM();
		isInteractiveObj->SetSnapRotation(CameraRot);
	}
}

void TLGameEngine::PlayerController::DropObject()
{
	//그랩한 오브젝트가 있을떄
	m_grabObject.reset();
	m_toggleRotateGrabObject = false;
}

void TLGameEngine::PlayerController::RayCastFromCamera()
{
	auto cameraPos = m_cameraTransform.lock()->GetWorldPosition();
	auto cameraDir = m_cameraTransform.lock()->GetForward();

	const float dist = 2.5f;

	const std::vector<std::string> filteredData = { "PLAYER", "TRIGGER"};

	auto ret = TLGameEngine::PhysicsManager::Instance().RayCast({ cameraPos.x,		cameraPos.y, cameraPos.z }, { cameraDir.x, cameraDir.y, cameraDir.z }, dist, filteredData, PhysicsEngine::ePhysicsType::INVAILD);
	m_previousRayHitObject = m_rayHitObject;
	m_rayHitObject = ret.m_gameObject;

	ActiveUI(3);
	if (m_rayHitObject.lock())
	{
		std::shared_ptr<InteractiveObject> interactiveComp = m_rayHitObject.lock()->GetComponent<InteractiveObject>();
		std::shared_ptr<ProjectorObject> projectorObjComp = m_rayHitObject.lock()->GetComponent<ProjectorObject>();

		if (interactiveComp) {
			auto currentObject = interactiveComp->GetCurrentStateObject();
			auto outliner = currentObject->GetComponent<OutLine>();

			if (outliner)
			{
				outliner->SetIsActive(true);
				std::string scene = SceneManager::Instance().m_currScene->GetName();
				if (scene == std::string("2_Level2.json"))
				{
					ActiveUI(2);
				}
				else if (interactiveComp->GetIs2DObject())
				{
					ActiveUI(0);
				}
				else if(m_grabObject.expired())
				{
					ActiveUI(2);
				}
				else
				{
					ActiveUI(1);
				}

			}

			if (!m_currentOutliner.expired() && (outliner != m_currentOutliner.lock()))
			{
				m_currentOutliner.lock()->SetIsActive(false);
				m_currentOutliner = outliner;
			}
			else
			{
				m_currentOutliner = outliner;
			}
		}
		else if (projectorObjComp) {
			auto currentObject = projectorObjComp->GetComponent<Transform>()->GetChild(0);
			auto outliner = currentObject->GetComponent<OutLine>();

			if (outliner)
			{
				outliner->SetIsActive(true);
				ActiveUI(0);
			}

			if (!m_currentOutliner.expired() && (outliner != m_currentOutliner.lock()))
			{
				m_currentOutliner.lock()->SetIsActive(false);
				m_currentOutliner = outliner;
			}
			else
			{
				m_currentOutliner = outliner;
			}
		}
		else
		{
			if (!m_currentOutliner.expired())
			{
				m_currentOutliner.lock()->SetIsActive(false);
				m_currentOutliner.reset();
			}
		}
	}
	else
	{
		if (!m_currentOutliner.expired())
		{
			m_currentOutliner.lock()->SetIsActive(false);
			m_currentOutliner.reset();
		}
	}
}

void TLGameEngine::PlayerController::ActiveUI(int type)
{
	switch (type)
	{
		case 0:
		{
			m_LButton.lock()->SetIsActive(true);
			m_RButton.lock()->SetIsActive(false);
			m_EButton.lock()->SetIsActive(false);
			break;
		}
		case 1:
		{
			m_LButton.lock()->SetIsActive(false);
			m_RButton.lock()->SetIsActive(true);
			m_EButton.lock()->SetIsActive(false);
			break;
		}
		case 2:
		{
			m_LButton.lock()->SetIsActive(false);
			m_RButton.lock()->SetIsActive(false);
			m_EButton.lock()->SetIsActive(true);
			break;
		}
		default:
		{
			m_LButton.lock()->SetIsActive(false);
			m_RButton.lock()->SetIsActive(false);
			m_EButton.lock()->SetIsActive(false);
			break;
		}
	}
}

