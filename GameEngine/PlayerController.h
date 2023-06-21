#pragma once
#include "Component.h"

namespace TLGameEngine
{
	class GameObject;

	class Transform;
	class Rigidbody;
	class Light;
	class FlashLight;
	class Material;
	class OutLine;
	class Text;

	class PlayerController :
		public Component
	{
	public:
		PlayerController();
		virtual ~PlayerController();

	public:
		// --------- Component Implement ------------
		virtual void Awake() override;
		virtual void OnEnable() override;
		virtual void OnDisalbe() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void FixedUpdate() override;
		virtual void OnDestroy() override;

	public:
		// --------- Getter/Setter Functions --------------
		void SetCamera(std::shared_ptr<TLGameEngine::GameObject> camera);

	public:
		// --------- Class Functions ----------------------
		void DropObject();

	private:
		void CheckInputs();

		void MoveCharacter();
		void GrabObject();
		void OnMouseMove(int buttonState, int x, int y, int middleOfSceenX, int middleOfSceenY);
		void RotateObject();
		void TurnOnFlashLight();

		void MaintainGrabObjectRotation();
		void RotateGrabObject(Vector2 mousePos);
		void SnapGrabObject();

		void RayCastFromCamera();

		void SetHoldMouseOnMiddle(bool value) { m_isHoldMouse = value; }

	private:

		const DirectX::SimpleMath::Vector4 m_holdPosition = { 0.f, 0.f, 3.f, 1.f };

		std::weak_ptr<TLGameEngine::Text> m_LButton;
		std::weak_ptr<TLGameEngine::Text> m_RButton;
		std::weak_ptr<TLGameEngine::Text> m_EButton;
		std::weak_ptr<TLGameEngine::Rigidbody> m_rigidBody;
		std::weak_ptr<TLGameEngine::Transform> m_Transform;

		std::weak_ptr<TLGameEngine::Transform> m_cameraTransform;

		// 이제 이것은 내것이다
		//std::weak_ptr <TLGameEngine::Light> m_flashLight;
		std::weak_ptr<TLGameEngine::FlashLight> m_flashLight;

		bool m_toggleRotateGrabObject;
		
		std::weak_ptr<TLGameEngine::GameObject> m_grabObject;

		std::weak_ptr<TLGameEngine::GameObject> m_rayHitObject;
		std::weak_ptr<TLGameEngine::OutLine> m_currentOutliner;

		std::weak_ptr<TLGameEngine::GameObject> m_previousRayHitObject;

		float m_transTimer;
	
		bool m_isHoldMouse = true;

		void ActiveUI(int type);
	private:
		FileID m_cameraID;
		FileID m_LButtonID;
		FileID m_RButtonID;
		FileID m_EButtonID;
		BOOST_DESCRIBE_CLASS(TLGameEngine::PlayerController, (), (), (), (m_cameraID, m_LButtonID, m_RButtonID, m_EButtonID))
	};
}