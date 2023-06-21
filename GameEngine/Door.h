#pragma once
#include "Component.h"

namespace TLGameEngine
{
	class Transform;
	class Material;
	class MeshRenderer;
	class AudioSource;

	class Door :
		public Component
	{
	public:
		Door();
		virtual ~Door();

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
		void SetDoorOpen(bool flag);
		void SetDoorEmissive(bool flag);

		void SetDoor(std::shared_ptr<GameObject> leftDoor, std::shared_ptr<GameObject> rightDoor);

	public:
		// --------- Class Functions ----------------------
		bool GetDoorOpen() { return m_isDoorOpen; }

	private:
		bool m_isDoorOpen;
		float m_DoorOpenLerp;

		Vector3 m_LeftDoorBasePosition;
		Vector3 m_RightDoorBasePosition;

		std::weak_ptr<Transform> m_leftDoor;
		std::weak_ptr<Transform> m_rightDoor;
		std::weak_ptr<MeshRenderer> m_frontLight;
		std::weak_ptr<MeshRenderer> m_backLight;
		std::shared_ptr<Material> m_openMaterial;
		std::shared_ptr<Material> m_closeMaterial;
		std::shared_ptr<AudioSource> m_audioSource;

	private:
		FileID m_leftDoorID;
		FileID m_rightDoorID;
		FileID m_frontLightID;
		FileID m_backLightID;
		FileInfo m_openMaterialID;
		FileInfo m_closeMaterialID;
		FileInfo m_openClipID;
		FileInfo m_closeClipID;

		Vector3 m_RightDoorDirection;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Door, (), (), (), (m_leftDoorID, m_rightDoorID, m_frontLightID, m_backLightID, m_openMaterialID, m_closeMaterialID, m_openClipID, m_closeClipID, m_RightDoorDirection))
	};

}