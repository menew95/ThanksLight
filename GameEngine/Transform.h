#pragma once

#include "Component.h"

namespace TLGameEngine
{
	const float Pi = 3.1415926535f;
	const float Rad2Deg = 360 / (Pi * 2);
	const float Deg2Rad = (Pi * 2) / 360;
	class Transform : public Component, public std::enable_shared_from_this<Transform>
	{
	public:
		Transform();
		virtual ~Transform();


	public:
		//virtual void PreUpdate()		override;
		virtual void Awake()			override;
		virtual void Update()			override;
		virtual void LateUpdate()		override;

		//virtual void Destroy()			override;

		//virtual void OnPreRender()		override;
		//virtual void OnRender()			override;
		//virtual void OnPostRender()		override;

		//svirtual void OnDrawGizmo()		override;

	public:

		void SetLocalPosition(DirectX::SimpleMath::Vector3& value);
		void SetLocalRotate(DirectX::SimpleMath::Vector3& value);
		void SetLocalRotateQ(DirectX::SimpleMath::Quaternion& value);
		void SetLocalScale(DirectX::SimpleMath::Vector3& value);
		void SetWorldPosition(DirectX::SimpleMath::Vector3& value);
		void SetWorldRotate(DirectX::SimpleMath::Vector3& value);
		void SetWorldRotateQ(DirectX::SimpleMath::Quaternion& value);
		void SetWorldScale(DirectX::SimpleMath::Vector3& value);

		void SetForward(const Vector3& forward) { m_forward = forward; }
		void SetUp(const Vector3& up) { m_up = up; }
		void SetRight(const Vector3& right) { m_right = right; }

		DirectX::SimpleMath::Vector3 GetLocalPosition();
		DirectX::SimpleMath::Vector3 GetLocalRotate();
		DirectX::SimpleMath::Quaternion GetLocalRotateQ();
		DirectX::SimpleMath::Vector3 GetLocalScale();

		DirectX::SimpleMath::Vector3 GetWorldPosition();
		DirectX::SimpleMath::Vector3 GetWorldRotate();
		DirectX::SimpleMath::Quaternion GetWorldRotateQ();
		DirectX::SimpleMath::Vector3 GetWorldScale();

		DirectX::SimpleMath::Matrix GetWorldTM();
		DirectX::SimpleMath::Matrix GetLocalTM();

		Vector3 GetForward() { return m_forward; }
		Vector3 GetUp() { return m_up; }
		Vector3 GetRight() { return m_right; }

		void Translate(float x, float y, float z);
		void Rotate(float x, float y, float z);
		void RotateQ(float x, float y, float z, float w);

		void RotateY(float val);
		void RotatePitch(float val);

		void Translate(DirectX::SimpleMath::Vector3 addPos);
		void Rotate(DirectX::SimpleMath::Vector3 addRot);
		void RotateQ(DirectX::SimpleMath::Quaternion addRotQ);

		void LookAt(GameObject* targetObject);
		void LookAt(DirectX::SimpleMath::Vector3 targetPosition);

		void AddChild(std::shared_ptr<Transform> child);
		void AddChild(GameObject* child);

		void InitParent(std::shared_ptr<Transform> parent);
		void InitChild(std::shared_ptr<Transform> child);
		void SetParent(std::shared_ptr<Transform> parent);

		void RemoveParent();
		void RemoveChild(std::shared_ptr<Transform> child);

		std::shared_ptr<Transform> GetParent();
		std::shared_ptr<Transform> GetChild(size_t i);
		std::shared_ptr<Transform> GetChild(std::string& name);
		const std::vector<std::shared_ptr<Transform>> GetChildren() const { return m_Childs; }

		// 부모 월드 TM 기준으로 local 갱신
		void UpdateParent();
		void UpdateChild();

		void UpdateMatrix();
	private:
		void UpdateTransform();

		void UpdateLocalTransform();
		void UpdateWorldTransform();

		void NormalizeAngles(DirectX::SimpleMath::Vector3 rot);
		float NormalizeAngle(float angle);

	protected:
		std::weak_ptr<Transform> m_Parent;
		std::vector<std::shared_ptr<Transform>> m_Childs;

		DirectX::SimpleMath::Vector3 m_forward;
		DirectX::SimpleMath::Vector3 m_right;
		DirectX::SimpleMath::Vector3 m_up;

		DirectX::SimpleMath::Vector3 m_LocalScale;
		DirectX::SimpleMath::Vector3 m_localEulerRotation;
		DirectX::SimpleMath::Quaternion m_LocalRotation;
		DirectX::SimpleMath::Vector3 m_LocalPosition;

		DirectX::SimpleMath::Matrix m_localPositionTM;
		DirectX::SimpleMath::Matrix m_localRotateTM;
		DirectX::SimpleMath::Matrix m_localScaleTM;

		DirectX::SimpleMath::Matrix m_localTM;

		DirectX::SimpleMath::Vector3 m_worldScale;
		DirectX::SimpleMath::Vector3 m_worldEulerRotation;
		DirectX::SimpleMath::Quaternion m_worldQuaternionRotation;
		DirectX::SimpleMath::Vector3 m_worldPosition;

		DirectX::SimpleMath::Matrix m_worldPositionTM;
		DirectX::SimpleMath::Matrix m_worldRotateTM;
		DirectX::SimpleMath::Matrix m_worldScaleTM;

		DirectX::SimpleMath::Matrix m_worldTM;

		//유니티 파싱용
	public:
		FileID GetFatherID() { return m_Father; }
		FileID GetGameObjectID() { return m_GameObject; }
		std::vector<FileID>& GetChildrenID() { return m_Children; }
	protected:
		FileID m_Father;
		std::vector<FileID> m_Children;
		FileID m_GameObject;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Transform, (), (), (m_LocalPosition, m_LocalRotation, m_LocalScale, m_Father, m_Children, m_GameObject), ())
	};
}