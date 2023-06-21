#pragma once
#include "Component.h"

//2D������Ʈ�� 3D������Ʈ�� �����ϴ� ����

namespace TLGameEngine
{
	class GameObject;

	class Transform;
	class Collider;
	class Rigidbody;
	class PlayerController;

	enum class eCurrentAxis
	{
		X_PLUS = 0,
		X_MINUS,
		Y_PLUS,
		Y_MINUS,
		Z_PLUS,
		Z_MINUS,
		FAILED
	};

	class InteractiveObject :
		public Component
	{
	public:
		InteractiveObject();
		virtual ~InteractiveObject();



	public:
		// --------- Component Implement ------------
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;


	public:
		// --------- Getter/Setter Functions --------------
		bool GetIs2DObject() const { return m_is2DObject; }
		bool GetIsConvert() const { return m_isConvert; }
		bool GetIsNotGrabbable() const { return m_IsNotGrabbable; }
		bool GetIsGrabed() const { return m_isGrabed; }

		std::shared_ptr<TLGameEngine::Collider> GetCollider() const { return m_collider.lock(); }
		std::shared_ptr<TLGameEngine::Rigidbody> GetRigidBody() const { return m_rigidBody.lock(); }
		std::shared_ptr<TLGameEngine::Transform> GetCurrent2DObjects() const { return m_current2DObjects.lock(); }
		std::shared_ptr<TLGameEngine::Transform> Get3DObjects() const { return m_3DObject.lock(); }

		std::shared_ptr<Transform> GetCurrentStateObject();

	public:
		// --------- Class Functions ----------------------
		void SetCamera(std::shared_ptr<GameObject> camera);

		//ī�޶� ���� �����̼� ������ ���
		void SetSnapRotation(const DirectX::SimpleMath::Matrix& CameraMatrix);
		void SetAxisRotation(const DirectX::SimpleMath::Matrix& CameraMatrix);

		void DropObject();
		void GrabObject(std::shared_ptr<Transform> holder);

		void Set3DObject();
		void Set2DObject();
		
		eCurrentAxis CalculateCurrent2DObject();

		void MoveToHoldPosition(Vector3 pos);

		void Set3DShape(std::shared_ptr<Transform> shape);
		void Set2DShape(std::shared_ptr<Transform> X_P, std::shared_ptr<Transform> X_M, std::shared_ptr<Transform> Y_P, std::shared_ptr<Transform> Y_M, std::shared_ptr<Transform> Z_P, std::shared_ptr<Transform> Z_M);

	private:
		//(2D������Ʈ�϶� ������� ���̴� ��.)
		void Billboard();
		//��������Ʈ�Ͻ� �����̼� ����.
		void Fixed();

		void LerpSnappedAxis();

		void SetInitializeState();
		void Set2DInit();

	private:
		std::weak_ptr<Transform> m_cameraTransform;
		
		std::weak_ptr<Collider> m_collider;
		std::weak_ptr<Rigidbody> m_rigidBody;

		Vector3 m_baseScale;

		//���� ��ġ , ������ġ		
		Quaternion m_previousQuarternion;
		Quaternion m_nowQuarternion;

		//������Ʈ�� �÷��̾�� �������°�
		bool m_isGrabed;
		std::weak_ptr<Transform> m_Holder;
		
		//������Ʈ�� 2D�ΰ� 3D�ΰ�
		bool m_is2DObject;

		//�ѹ� ���ϴ� ������Ʈ��... 
		bool m_isConvert = false;

		//ȸ�� ���� ���� ��
		//0, 90, -90, 180  // 4 * 4 * 4���� ������ �ִ�.
		int m_AxisAlignX;
		int m_AxisAlignY;
		int m_AxisAlignZ;

		//ȸ�� �������. ����
		bool m_isSnapping;
		float m_isSnappingLerp;

		//IsWallObject
		//��������Ʈ�� �ѹ��� ��ȯ����. �����嵵 �ƴ�.

		//�࿡ ���� On Off �Ǿ��� 2D ������ ������Ʈ���� ����.
		//X_PLUS = 0,
		//X_MINUS,
		//Y_PLUS,
		//Y_MINUS,
		//Z_PLUS,
		//Z_MINUS == 5
		std::weak_ptr<Transform> m_current2DObjects;
		std::weak_ptr<Transform> m_2DObjects[6];

		std::weak_ptr<Transform> m_3DObject;

	private:
		FileID m_cameraID;
		
		FileID m_2DObjectXP;
		FileID m_2DObjectXM;
		FileID m_2DObjectYP;
		FileID m_2DObjectYM;
		FileID m_2DObjectZP;
		FileID m_2DObjectZM;

		FileID m_3DObjectID;
		
		//0�� 3D, 1�� XP���� .... ����
		int m_StartShape;
		int m_IsOnewayObject;
		int m_IsWallObject;

		int m_IsNotGrabbable;


		BOOST_DESCRIBE_CLASS(TLGameEngine::InteractiveObject, (), (), (), (m_cameraID, m_2DObjectXP, m_2DObjectXM, m_2DObjectYP, m_2DObjectYM, m_2DObjectZP, m_2DObjectZM, m_3DObjectID, m_StartShape, m_IsOnewayObject, m_IsWallObject, m_IsNotGrabbable))
	};
}