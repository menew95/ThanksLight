#include "pch.h"
#include "InteractiveObject.h"

#include "GameObject.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "MeshRenderer.h";

#include "SceneManager.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"

#include "PlayerController.h"

#include <iostream>

TLGameEngine::InteractiveObject::InteractiveObject() :
	Component(Component::Type::Action),
	m_AxisAlignX(2),
	m_AxisAlignY(0),
	m_AxisAlignZ(0),
	m_isGrabed(false),
	m_is2DObject(false),
	m_isSnapping(false),
	m_isSnappingLerp(0.0f)
{
}

TLGameEngine::InteractiveObject::~InteractiveObject()
{

}

void TLGameEngine::InteractiveObject::Awake()
{
	m_cameraTransform = SceneManager::Instance().FindObject(m_cameraID)->GetComponent<Transform>();

	m_2DObjects[0] = SceneManager::Instance().FindObject(m_2DObjectXP)->GetComponent<Transform>();
	m_2DObjects[1] = SceneManager::Instance().FindObject(m_2DObjectXM)->GetComponent<Transform>();
	m_2DObjects[2] = SceneManager::Instance().FindObject(m_2DObjectYP)->GetComponent<Transform>();
	m_2DObjects[3] = SceneManager::Instance().FindObject(m_2DObjectYM)->GetComponent<Transform>();
	m_2DObjects[4] = SceneManager::Instance().FindObject(m_2DObjectZP)->GetComponent<Transform>();
	m_2DObjects[5] = SceneManager::Instance().FindObject(m_2DObjectZM)->GetComponent<Transform>();

	m_3DObject = SceneManager::Instance().FindObject(m_3DObjectID)->GetComponent<Transform>();


	m_collider = m_gameObject.lock()->GetComponent<BoxCollider>();
	if (m_collider.expired())
	{
		m_collider = m_gameObject.lock()->GetComponent<SphereCollider>();
		if (m_collider.expired())
		{
			m_collider = m_gameObject.lock()->GetComponent<CapsuleCollider>();
			if (m_collider.expired())
			{
				m_collider = m_gameObject.lock()->GetComponent<MeshCollider>();
				if (m_collider.expired())
				{
					assert(false);
				}
			}
		}
	}

	m_rigidBody = m_gameObject.lock()->GetComponent<Rigidbody>();
	m_baseScale = GetTransform()->GetLocalScale();


}

void TLGameEngine::InteractiveObject::Start()
{
	SetInitializeState();
}

void TLGameEngine::InteractiveObject::Update()
{
	if (m_is2DObject)
	{
		if (!m_IsWallObject)
		{
			Billboard();
		}
		else
		{
			Fixed();
		}
	}

	LerpSnappedAxis();
}

void TLGameEngine::InteractiveObject::SetAxisRotation(const DirectX::SimpleMath::Matrix& CameraMatrix)
{
	if (m_AxisAlignX > 2 || m_AxisAlignY > 2 || m_AxisAlignZ > 2)
	{
		assert(false);
	}

	Vector3 AxisAlignedRotation = { m_AxisAlignX * 1.5708f, m_AxisAlignY * 1.5708f ,m_AxisAlignZ * 1.5708f };

	auto cameraMatrix = m_cameraTransform.lock()->GetWorldTM();
	auto grabWorldRot = GetTransform()->GetWorldTM();
	auto cameraWorldTMInverse = cameraMatrix.Invert();

	auto grabObjLocalTM = grabWorldRot * cameraWorldTMInverse;

	DirectX::SimpleMath::Vector3 localscl;
	DirectX::SimpleMath::Quaternion localrot;
	DirectX::SimpleMath::Vector3 localpos;

	grabObjLocalTM.Decompose(localscl, localrot, localpos);

	DirectX::SimpleMath::Quaternion nowQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(AxisAlignedRotation);

	auto localTM = DirectX::SimpleMath::Matrix::CreateScale(localscl) * DirectX::SimpleMath::Matrix::CreateFromQuaternion(nowQuat) * DirectX::SimpleMath::Matrix::CreateTranslation(localpos);
	auto WorldTM = localTM * cameraMatrix;

	DirectX::SimpleMath::Vector3 worldscl;
	DirectX::SimpleMath::Quaternion worldrot;
	DirectX::SimpleMath::Vector3 worldpos;

	WorldTM.Decompose(worldscl, worldrot, worldpos);

	GetTransform()->SetWorldScale(worldscl);
	GetTransform()->SetWorldRotateQ(worldrot);
	GetTransform()->SetWorldPosition(worldpos);

}

void TLGameEngine::InteractiveObject::DropObject()
{
	m_isGrabed = false;

	if (m_is2DObject)
	{
		m_collider.lock()->SetColliderLayer("2DOBJECT");
	}
	else
	{
		m_collider.lock()->SetColliderLayer("3DOBJECT");
	}

	//m_rigidBody.lock()->SetAngularVelocity({ 0.f, 0.f, 0.f });
	//m_rigidBody.lock()->SetLinearVelocity({ 0.f, 0.f, 0.f });

	if (m_is2DObject)
	{
		m_rigidBody.lock()->SetGravity(false);
	}
	else
	{
		m_rigidBody.lock()->SetGravity(true);
	}

	if (m_Holder.lock())
	{
		auto PlayerComp = m_Holder.lock()->GetComponent<PlayerController>();

		if (PlayerComp)
		{
			PlayerComp->DropObject();
		}
	}

	m_Holder.reset();
}

void TLGameEngine::InteractiveObject::GrabObject(std::shared_ptr<Transform> holder)
{
	m_isGrabed = true;

	m_Holder = holder;

	m_collider.lock()->SetColliderLayer("HANDEDOBJECT");
	m_rigidBody.lock()->SetAngularVelocity({ 0.f, 0.f, 0.f });
	m_rigidBody.lock()->SetLinearVelocity({ 0.f, 0.f, 0.f });

	m_rigidBody.lock()->SetGravity(false);
}

std::shared_ptr<TLGameEngine::Transform> TLGameEngine::InteractiveObject::GetCurrentStateObject()
{
	if (m_is2DObject)
	{
		return m_current2DObjects.lock();
	}
	else
	{
		return m_3DObject.lock();
	}
}

void TLGameEngine::InteractiveObject::SetCamera(std::shared_ptr<GameObject> camera)
{
	m_cameraTransform = camera->GetComponent<Transform>();
}

void TLGameEngine::InteractiveObject::SetSnapRotation(const DirectX::SimpleMath::Matrix& CameraMatrix)
{
	auto grabWorldRot = GetTransform()->GetWorldTM();
	auto CameraWorldTMInverse = CameraMatrix.Invert();

	auto grabObjLocalTM = grabWorldRot * CameraWorldTMInverse;

	Vector3 localscl;
	Quaternion localrot;
	Vector3 localpos;

	grabObjLocalTM.Decompose(localscl, localrot, localpos);

	auto eulerLocalrot = localrot.ToEuler();

	//rad
	m_AxisAlignX = (int)roundf(eulerLocalrot.x / 0.785398f);
	m_AxisAlignY = (int)roundf(eulerLocalrot.y / 0.785398f);
	m_AxisAlignZ = (int)roundf(eulerLocalrot.z / 0.785398f);

	m_AxisAlignX /= 2;
	m_AxisAlignY /= 2;
	m_AxisAlignZ /= 2;

	if (m_AxisAlignX > 2 || m_AxisAlignY > 2 || m_AxisAlignZ > 2)
	{
		assert(false);
	}

	Vector3 AxisAlignedRotation = { m_AxisAlignX * 1.5708f, m_AxisAlignY * 1.5708f ,m_AxisAlignZ * 1.5708f };

	m_previousQuarternion = localrot;
	m_nowQuarternion = Quaternion::CreateFromYawPitchRoll(AxisAlignedRotation);

	m_isSnapping = true;
	m_isSnappingLerp = 0.f;
}

void TLGameEngine::InteractiveObject::Set3DObject()
{
	if (!m_isConvert)
	{
		if (m_is2DObject == true)
		{
			m_is2DObject = false;

			m_rigidBody.lock()->SetGravity(true);

			auto cameraTransform = m_cameraTransform.lock()->GetWorldPosition();
			auto transform = GetTransform()->GetWorldPosition();


			auto cameraTM = m_cameraTransform.lock()->GetWorldTM();

			for (int axis = 0; axis < 6; axis++)
			{
				m_2DObjects[axis].lock()->GetGameObject()->GetComponent<MeshRenderer>()->SetIsActive(false);
			}
			m_current2DObjects.reset();

			m_3DObject.lock()->GetGameObject()->GetComponent<MeshRenderer>()->SetIsActive(true);

			if (!m_IsWallObject)
			{
				auto dir = transform - cameraTransform;
				dir.Normalize();
				GetTransform()->Translate({ dir * m_baseScale });
				SetAxisRotation(cameraTM);
			}

			m_collider.lock()->SetColliderLayer("3DOBJECT");
			DropObject();
		}
	}

	if (m_IsOnewayObject)
	{
		if (!m_isConvert)
		{
			m_isConvert = true;
		}
	}

	if (m_IsWallObject)
	{
		if (!m_isConvert)
		{
			m_isConvert = true;
			m_IsOnewayObject = true;
			auto cameraTransform = m_cameraTransform.lock()->GetWorldPosition();
			auto transform = GetTransform()->GetWorldPosition();

			auto dir = transform - cameraTransform;
		}
	}

}

void TLGameEngine::InteractiveObject::Set2DObject()
{

	if (!m_isConvert)
	{
		if (m_is2DObject == false)
		{
			m_is2DObject = true;

			m_rigidBody.lock()->SetGravity(false);

			auto CameraTransform = m_cameraTransform.lock()->GetWorldPosition();
			auto transform = GetTransform()->GetWorldPosition();
			auto transformTM = GetTransform()->GetWorldTM();

			auto current2DObjectAxis = CalculateCurrent2DObject();

			for (int axis = 0; axis < 6; axis++)
			{
				m_2DObjects[axis].lock()->GetGameObject()->GetComponent<MeshRenderer>()->SetIsActive(false);
			}

			for (int axis = 0; axis < 6; axis++)
			{
				if (axis == (int)current2DObjectAxis)
				{
					m_current2DObjects = m_2DObjects[axis];
					m_2DObjects[axis].lock()->GetGameObject()->GetComponent<MeshRenderer>()->SetIsActive(true);
				}
			}

			m_3DObject.lock()->GetGameObject()->GetComponent<MeshRenderer>()->SetIsActive(false);

			auto dir = CameraTransform - transform;
			dir.Normalize();
			GetTransform()->Translate({ dir * m_baseScale });

			//DirectX::SimpleMath::Vector3 scale2d = { m_baseScale.x *0.87f , m_baseScale.y * 0.97f , 0.0005f };
			//GetTransform()->SetLocalScale(scale2d);


			m_collider.lock()->SetColliderLayer("2DOBJECT");

			DropObject();
		}
	}

	if (m_IsOnewayObject)
	{
		if (!m_isConvert)
		{
			m_isConvert = true;
		}
		else
		{
			return;
		}
	}

}

TLGameEngine::eCurrentAxis TLGameEngine::InteractiveObject::CalculateCurrent2DObject()
{
	Vector3 AxisAlignedRotation = { m_AxisAlignX * 1.5708f, m_AxisAlignY * 1.5708f ,m_AxisAlignZ * 1.5708f };

	auto cameraRot = m_cameraTransform.lock()->GetWorldRotate();
	auto objRot = GetTransform()->GetWorldRotate();
	auto objectMatrix = DirectX::XMMatrixRotationRollPitchYaw(objRot.x, objRot.y, objRot.z);
	auto cameraforward = m_cameraTransform.lock()->GetForward();
	cameraforward.Normalize();

	Vector4 resultAxis[6];

	resultAxis[0] = DirectX::XMVector3Transform({ +1.f, +0.f, +0.f, 0.f }, objectMatrix);
	resultAxis[1] = DirectX::XMVector3Transform({ -1.f, +0.f, +0.f, 0.f }, objectMatrix);
	resultAxis[2] = DirectX::XMVector3Transform({ +0.f, +1.f, +0.f, 0.f }, objectMatrix);
	resultAxis[3] = DirectX::XMVector3Transform({ +0.f, -1.f, +0.f, 0.f }, objectMatrix);
	resultAxis[4] = DirectX::XMVector3Transform({ +0.f, +0.f, +1.f, 0.f }, objectMatrix);
	resultAxis[5] = DirectX::XMVector3Transform({ +0.f, +0.f, -1.f, 0.f }, objectMatrix);

	Vector3 axis[6];

	const float epsilon = 0.1f;

	for (int i = 0; i < 6; i++)
	{
		XMStoreFloat3(&axis[i], resultAxis[i]);
		axis[i] -= cameraforward;

		if ((-epsilon < axis[i].x && epsilon > axis[i].x) && (-epsilon < axis[i].y && epsilon > axis[i].y) && (-epsilon < axis[i].z && epsilon > axis[i].z))
		{
			std::cout << i << std::endl;
			return (TLGameEngine::eCurrentAxis)i;
		}
	}

	return TLGameEngine::eCurrentAxis::X_MINUS;
}

void TLGameEngine::InteractiveObject::MoveToHoldPosition(Vector3 pos)
{
	m_rigidBody.lock()->SetGravity(false);
	m_rigidBody.lock()->SetAngularVelocity({ 0.f, 0.f, 0.f });
	m_rigidBody.lock()->SetLinearVelocity({ pos.x * 1.5f,  pos.y * 1.5f, pos.z * 1.5f });
}

void TLGameEngine::InteractiveObject::Set3DShape(std::shared_ptr<Transform> shape)
{
	m_3DObject = shape;
}

void TLGameEngine::InteractiveObject::Set2DShape(std::shared_ptr<Transform> X_P, std::shared_ptr<Transform> X_M, std::shared_ptr<Transform> Y_P, std::shared_ptr<Transform> Y_M, std::shared_ptr<Transform> Z_P, std::shared_ptr<Transform> Z_M)
{
	m_2DObjects[0] = X_P;
	m_2DObjects[1] = X_M;
	m_2DObjects[2] = Y_P;
	m_2DObjects[3] = Y_M;
	m_2DObjects[4] = Z_P;
	m_2DObjects[5] = Z_M;
}

void TLGameEngine::InteractiveObject::Billboard()
{
	auto CameraRot = m_cameraTransform.lock()->GetWorldRotate();

	Vector3 temp = { 1.5708f, 0.f, 0.f };

	auto axisRotTM = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(temp);

	auto CameraRotTM = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(CameraRot);

	CameraRotTM.m[0][0] *= -1;
	CameraRotTM.m[0][1] *= -1;
	CameraRotTM.m[0][2] *= -1;
	CameraRotTM.m[2][0] *= -1;
	CameraRotTM.m[2][1] *= -1;
	CameraRotTM.m[2][2] *= -1;

	auto resultTM = axisRotTM * CameraRotTM;

	DirectX::SimpleMath::Vector3 scl;
	DirectX::SimpleMath::Quaternion rot;
	DirectX::SimpleMath::Vector3 pos;

	resultTM.Decompose(scl, rot, pos);

	GetTransform()->SetWorldRotateQ(rot);
}


void TLGameEngine::InteractiveObject::Fixed()
{

	Vector3 AxisAlignedRotation = { m_AxisAlignX * 1.5708f, m_AxisAlignY * 1.5708f ,m_AxisAlignZ * 1.5708f };

	GetTransform()->SetWorldRotate(AxisAlignedRotation);

}

void TLGameEngine::InteractiveObject::LerpSnappedAxis()
{
	if (m_isGrabed)
	{
		if (m_isSnapping)
		{
			float lerpTime = 5.0f;
			float deltaTime = TLGameEngine::Time::Instance().GetDeltaTime() * lerpTime;
			m_isSnappingLerp += deltaTime;

			auto nowQuat = DirectX::SimpleMath::Quaternion::Slerp(m_previousQuarternion, m_nowQuarternion, m_isSnappingLerp);

			////m_deltarot* deltaTime
			//auto nowRot = m_previousRot + m_deltarot * m_isSnappingLerp;

			auto cameraMatrix = m_cameraTransform.lock()->GetWorldTM();
			auto grabWorldRot = GetTransform()->GetWorldTM();
			auto cameraWorldTMInverse = cameraMatrix.Invert();

			auto grabObjLocalTM = grabWorldRot * cameraWorldTMInverse;

			DirectX::SimpleMath::Vector3 localscl;
			DirectX::SimpleMath::Quaternion localrot;
			DirectX::SimpleMath::Vector3 localpos;

			grabObjLocalTM.Decompose(localscl, localrot, localpos);

			auto localTM = DirectX::SimpleMath::Matrix::CreateScale(localscl) * DirectX::SimpleMath::Matrix::CreateFromQuaternion(nowQuat) * DirectX::SimpleMath::Matrix::CreateTranslation(localpos);
			auto WorldTM = localTM * cameraMatrix;

			DirectX::SimpleMath::Vector3 worldscl;
			DirectX::SimpleMath::Quaternion worldrot;
			DirectX::SimpleMath::Vector3 worldpos;

			WorldTM.Decompose(worldscl, worldrot, worldpos);

			//GetTransform()->SetWorldScale(worldscl);
			GetTransform()->SetWorldRotateQ(worldrot);
			//GetTransform()->SetWorldPosition(worldpos);

			if (m_isSnappingLerp > 1.0f)
			{
				m_isSnapping = false;
				m_isSnappingLerp = 0.f;
			}
		}
	}
}

void TLGameEngine::InteractiveObject::SetInitializeState()
{
	switch (m_StartShape)
	{
	case 0:
	{
		m_AxisAlignZ = +1; //XP
		Set2DInit();
		break;
	}
	case 1:
	{
		m_AxisAlignZ = -1; //XM
		Set2DInit();
		break;
	}
	case 2:
	{
		m_AxisAlignZ = 2; //YP
		Set2DInit();
		break;
	}
	case 3:
	{
		//YM
		Set2DInit();
		break;
	}
	case 4:
	{
		m_AxisAlignX = 1; //ZP
		Set2DInit();
		break;
	}
	case 5:
	{
		m_AxisAlignX = -1;
		Set2DInit();
		break; //ZM
	}
	default:
		Set3DObject();
		if (m_IsWallObject)
		{
			m_collider.lock()->SetColliderLayer("WALLOBJECT");
		}
		break;
	}


	m_isConvert = false;

}

void TLGameEngine::InteractiveObject::Set2DInit()
{
	if (m_is2DObject == false)
	{
		m_is2DObject = true;
		m_rigidBody.lock()->SetGravity(false);

		int current2DObjectAxis = m_StartShape;


		for (int axis = 0; axis < 6; axis++)
		{
			m_2DObjects[axis].lock()->GetGameObject()->GetComponent<MeshRenderer>()->SetIsActive(false);
		}

		for (int axis = 0; axis < 6; axis++)
		{
			if (axis == current2DObjectAxis)
			{
				m_current2DObjects = m_2DObjects[axis];
				m_2DObjects[axis].lock()->GetGameObject()->GetComponent<MeshRenderer>()->SetIsActive(true);
			}
		}

		m_3DObject.lock()->GetGameObject()->GetComponent<MeshRenderer>()->SetIsActive(false);

		if (m_IsWallObject)
		{
			m_collider.lock()->SetColliderLayer("WALLOBJECT");
		}
		else
		{
			m_collider.lock()->SetColliderLayer("2DOBJECT");
		}
	}
}
