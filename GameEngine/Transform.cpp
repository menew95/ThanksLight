#include "pch.h"
#include "Transform.h"
#include "GameObject.h"

TLGameEngine::Transform::Transform()
	: Component(Type::Action)
{
	m_LocalPosition = { 0,0,0 };
	m_localEulerRotation = { 0,0,0 };
	m_LocalScale = { 1,1,1 };

	m_worldPosition = { 0,0,0 };
	m_worldEulerRotation = { 0,0,0 };
	m_worldScale = { 1,1,1 };

	m_forward = { 0, 0, 1 };
	m_right = { 1, 0, 0 };
	m_up = { 0, 1, 0 };

	m_localTM = DirectX::XMMatrixIdentity();
	m_localPositionTM = DirectX::XMMatrixIdentity();
	m_localRotateTM = DirectX::XMMatrixIdentity();
	m_localScaleTM = DirectX::XMMatrixIdentity();
	m_worldTM = DirectX::XMMatrixIdentity();
	m_worldPositionTM = DirectX::XMMatrixIdentity();
	m_worldRotateTM = DirectX::XMMatrixIdentity();
	m_worldScaleTM = DirectX::XMMatrixIdentity();
}

TLGameEngine::Transform::~Transform()
{

}

void TLGameEngine::Transform::SetParent(std::shared_ptr<Transform> parent)
{
	if (parent == nullptr || m_Parent.lock() == parent)
	{
		return;
	}
	m_Parent = parent;

	m_Parent.lock()->AddChild(shared_from_this());
}

void TLGameEngine::Transform::RemoveParent()
{
	m_LocalScale = m_worldScale;
	m_localEulerRotation = m_worldEulerRotation;
	m_LocalRotation = m_worldQuaternionRotation;
	m_LocalPosition = m_worldPosition;

	m_Parent.lock()->RemoveChild(shared_from_this());

	m_Parent.reset();
}

void TLGameEngine::Transform::RemoveChild(std::shared_ptr<Transform> childObject)
{
	auto iter = m_Childs.begin();

	m_Childs.erase(
		std::remove(
			m_Childs.begin(), m_Childs.end(), childObject
		), m_Childs.end());
}

std::shared_ptr<TLGameEngine::Transform> TLGameEngine::Transform::GetParent()
{
	return m_Parent.lock();
}

std::shared_ptr<TLGameEngine::Transform> TLGameEngine::Transform::GetChild(size_t i)
{
	if (i < m_Childs.size())
	{
		return m_Childs[i];
	}

	return nullptr;
}

std::shared_ptr<TLGameEngine::Transform> TLGameEngine::Transform::GetChild(std::string& name)
{
	for (auto& child : m_Childs)
	{
		if (child->m_gameObject.lock()->GetName() == name)
		{
			return child;
		}
	}

	return nullptr;
}

void TLGameEngine::Transform::UpdateTransform()
{
	// world tm 갱신
	if (m_Parent.lock() != nullptr)
	{
		DirectX::SimpleMath::Matrix parentTM = m_Parent.lock()->GetWorldTM();
		m_worldTM = m_localTM * parentTM;


		m_worldPositionTM = m_localPositionTM * parentTM;
		m_worldRotateTM = m_localRotateTM * parentTM;
		m_worldScaleTM = m_localScaleTM * parentTM;

		m_worldTM.Decompose(m_worldScale, m_worldQuaternionRotation, m_worldPosition);

		m_worldEulerRotation = m_worldQuaternionRotation.ToEuler();
	}
	else
	{
		m_worldTM = m_localTM;

		m_worldPositionTM = m_localPositionTM;
		m_worldRotateTM = m_localRotateTM;
		m_worldScaleTM = m_localScaleTM;

		m_worldPosition = m_LocalPosition;
		m_worldEulerRotation = m_worldEulerRotation;
		m_worldQuaternionRotation = m_LocalRotation;
		m_worldScale = m_LocalScale;
	}

	m_forward = -m_worldRotateTM.Forward();
	m_up = m_worldRotateTM.Up();
	m_right = m_worldRotateTM.Right();

	UpdateChild();
}

void TLGameEngine::Transform::UpdateMatrix()
{
	m_localPositionTM._41 = m_LocalPosition.x;
	m_localPositionTM._42 = m_LocalPosition.y;
	m_localPositionTM._43 = m_LocalPosition.z;

	m_localEulerRotation = m_LocalRotation.ToEuler();

	m_localRotateTM = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_LocalRotation);

	m_localScaleTM._11 = m_LocalScale.x;
	m_localScaleTM._22 = m_LocalScale.y;
	m_localScaleTM._33 = m_LocalScale.z;

	m_localTM = m_localScaleTM * m_localRotateTM * m_localPositionTM;

	UpdateTransform();
}

void TLGameEngine::Transform::UpdateLocalTransform()
{
	// Local TM 재조립 및 worldTM 갱신후

	m_localTM = m_localScaleTM * m_localRotateTM * m_localPositionTM;

	if (m_Parent.lock() != nullptr)
	{
		// worldTM decompose, world position rotation scale 갱신

		DirectX::SimpleMath::Matrix parentTM = m_Parent.lock()->GetWorldTM();
		m_worldTM = m_localTM * parentTM;

		m_worldTM.Decompose(m_worldScale, m_worldQuaternionRotation, m_worldPosition);

		m_worldEulerRotation = m_worldQuaternionRotation.ToEuler();
	}
	else
	{
		// local == world

		m_worldTM = m_localTM;

		m_worldScale = m_LocalScale;
		m_worldEulerRotation = m_localEulerRotation;
		m_worldQuaternionRotation = m_LocalRotation;
		m_worldPosition = m_LocalPosition;
	}

	m_worldPositionTM = DirectX::SimpleMath::Matrix::CreateTranslation(m_worldPosition);
	m_worldRotateTM = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_worldQuaternionRotation);
	m_worldScaleTM = DirectX::SimpleMath::Matrix::CreateScale(m_worldScale);

	m_forward = -m_worldRotateTM.Forward();
	m_up = m_worldRotateTM.Up();
	m_right = m_worldRotateTM.Right();

	UpdateChild();
}

void TLGameEngine::Transform::UpdateWorldTransform()
{
	// World TM 갱신후 localTM 갱신

	m_worldTM = m_worldScaleTM * m_worldRotateTM * m_worldPositionTM;

	if (m_Parent.lock() != nullptr)
	{
		DirectX::SimpleMath::Matrix parentTM = m_Parent.lock()->GetWorldTM().Invert();
		m_localTM = m_worldTM * parentTM;

		m_localTM.Decompose(m_LocalScale, m_LocalRotation, m_LocalPosition);

		m_localEulerRotation = m_LocalRotation.ToEuler();
	}
	else
	{
		// world == local
		m_localTM = m_worldTM;

		m_LocalScale = m_worldScale;
		m_localEulerRotation = m_worldEulerRotation;
		m_LocalRotation = m_worldQuaternionRotation;
		m_LocalPosition = m_worldPosition;
	}

	m_localPositionTM = DirectX::SimpleMath::Matrix::CreateTranslation(m_LocalPosition);
	m_localRotateTM = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_LocalRotation);
	m_localScaleTM = DirectX::SimpleMath::Matrix::CreateScale(m_LocalScale);

	m_forward = -m_worldRotateTM.Forward();
	m_up = m_worldRotateTM.Up();
	m_right = m_worldRotateTM.Right();

	UpdateChild();
}

void TLGameEngine::Transform::UpdateParent()
{
	DirectX::SimpleMath::Matrix parentTM = m_Parent.lock()->GetWorldTM().Invert();

	m_localTM = m_worldTM * parentTM;

	m_localTM.Decompose(m_LocalScale, m_LocalRotation, m_LocalPosition);

	UpdateTransform();
}

void TLGameEngine::Transform::UpdateChild()
{
	for (auto child : m_Childs)
	{
		child->UpdateTransform();
	}
}

void TLGameEngine::Transform::AddChild(std::shared_ptr<Transform> child)
{
	m_Childs.push_back(child);

	child->UpdateParent();
}

void TLGameEngine::Transform::InitParent(std::shared_ptr<Transform> parent)
{
	if (parent == nullptr || m_Parent.lock() == parent)
	{
		return;
	}
	m_Parent = parent;
	m_Parent.lock()->InitChild(shared_from_this());

	UpdateTransform();
}

void TLGameEngine::Transform::InitChild(std::shared_ptr<Transform> child)
{
	m_Childs.push_back(child);
}

void TLGameEngine::Transform::AddChild(GameObject* child)
{
	auto chilsTransform = child->GetComponent<Transform>();

	m_Childs.push_back(chilsTransform);

	chilsTransform->UpdateParent();
}

void TLGameEngine::Transform::Awake()
{
	UpdateMatrix();
}

void TLGameEngine::Transform::Update()
{
}

void TLGameEngine::Transform::LateUpdate()
{
	UpdateLocalTransform();
}

void TLGameEngine::Transform::SetLocalPosition(DirectX::SimpleMath::Vector3& value)
{
	// localPosition transform 갱신

	m_LocalPosition = value;

	m_localPositionTM._41 = value.x;
	m_localPositionTM._42 = value.y;
	m_localPositionTM._43 = value.z;


	UpdateLocalTransform();
}

void TLGameEngine::Transform::SetLocalRotate(DirectX::SimpleMath::Vector3& value)
{
	// localRotation transform 갱신

	m_localEulerRotation = value;

	m_LocalRotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_localEulerRotation);

	m_localRotateTM = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_LocalRotation);

	UpdateLocalTransform();
}

void TLGameEngine::Transform::SetLocalRotateQ(DirectX::SimpleMath::Quaternion& value)
{
	// localRotation transform 갱신

	m_LocalRotation = value;

	m_localEulerRotation = m_LocalRotation.ToEuler();

	m_localRotateTM = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_LocalRotation);

	UpdateLocalTransform();
}

void TLGameEngine::Transform::SetLocalScale(DirectX::SimpleMath::Vector3& value)
{
	// localScale transform 갱신

	m_LocalScale = value;

	m_localScaleTM._11 = value.x;
	m_localScaleTM._22 = value.y;
	m_localScaleTM._33 = value.z;

	UpdateLocalTransform();
}

void TLGameEngine::Transform::SetWorldPosition(DirectX::SimpleMath::Vector3& value)
{
	m_worldPosition = value;

	m_worldPositionTM = DirectX::SimpleMath::Matrix::CreateTranslation(m_worldPosition);

	UpdateWorldTransform();
}

void TLGameEngine::Transform::SetWorldRotate(DirectX::SimpleMath::Vector3& value)
{
	m_worldEulerRotation = value;

	m_worldQuaternionRotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_worldEulerRotation);

	m_worldRotateTM = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_worldQuaternionRotation);

	UpdateWorldTransform();
}

void TLGameEngine::Transform::SetWorldRotateQ(DirectX::SimpleMath::Quaternion& value)
{
	m_worldQuaternionRotation = value;

	m_worldEulerRotation = m_worldQuaternionRotation.ToEuler();

	m_worldRotateTM = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_worldQuaternionRotation);

	UpdateWorldTransform();
}

void TLGameEngine::Transform::SetWorldScale(DirectX::SimpleMath::Vector3& value)
{
	m_worldScale = value;

	m_worldPositionTM = DirectX::SimpleMath::Matrix::CreateScale(value);

	UpdateWorldTransform();
}

DirectX::SimpleMath::Vector3 TLGameEngine::Transform::GetLocalPosition()
{
	return m_LocalPosition;
}

DirectX::SimpleMath::Vector3 TLGameEngine::Transform::GetLocalRotate()
{
	return m_localEulerRotation;
}

DirectX::SimpleMath::Quaternion TLGameEngine::Transform::GetLocalRotateQ()
{
	return m_LocalRotation;
}

DirectX::SimpleMath::Vector3 TLGameEngine::Transform::GetLocalScale()
{
	return m_LocalScale;
}

DirectX::SimpleMath::Vector3 TLGameEngine::Transform::GetWorldPosition()
{
	return m_worldPosition;
}

DirectX::SimpleMath::Vector3 TLGameEngine::Transform::GetWorldRotate()
{
	return m_worldEulerRotation;
}

DirectX::SimpleMath::Quaternion TLGameEngine::Transform::GetWorldRotateQ()
{
	return m_worldQuaternionRotation;
}

DirectX::SimpleMath::Vector3 TLGameEngine::Transform::GetWorldScale()
{
	return m_worldScale;
}

DirectX::SimpleMath::Matrix TLGameEngine::Transform::GetWorldTM()
{
	return m_worldTM;
}

DirectX::SimpleMath::Matrix TLGameEngine::Transform::GetLocalTM()
{
	return m_localTM;
}

void TLGameEngine::Transform::Translate(float x, float y, float z)
{
	m_LocalPosition += {x, y, z};

	SetLocalPosition(m_LocalPosition);
}

void TLGameEngine::Transform::Rotate(float x, float y, float z)
{
	m_localEulerRotation += {x, y, z};

	SetLocalRotate(m_localEulerRotation);
}

void TLGameEngine::Transform::RotateQ(float x, float y, float z, float w)
{
	m_LocalRotation += {x, y, z, w};

	SetLocalRotateQ(m_LocalRotation);
}

void TLGameEngine::Transform::RotateY(float val)
{
}

void TLGameEngine::Transform::RotatePitch(float val)
{
}

void TLGameEngine::Transform::NormalizeAngles(DirectX::SimpleMath::Vector3 rot)
{
	rot.x = NormalizeAngle(rot.x);
	rot.y = NormalizeAngle(rot.y);
	rot.z = NormalizeAngle(rot.z);
}

float TLGameEngine::Transform::NormalizeAngle(float angle)
{
	if (angle > 360)
	{
		angle -= 360;
	}
	else if (angle < 0)
	{
		angle += 360;
	}

	return angle;
}

void TLGameEngine::Transform::Translate(DirectX::SimpleMath::Vector3 addPos)
{
	m_LocalPosition += addPos;

	SetLocalPosition(m_LocalPosition);
}

void TLGameEngine::Transform::Rotate(DirectX::SimpleMath::Vector3 addRot)
{
	m_localEulerRotation += addRot;

	SetLocalRotate(m_localEulerRotation);
}

void TLGameEngine::Transform::RotateQ(DirectX::SimpleMath::Quaternion addRotQ)
{
	m_LocalRotation += addRotQ;

	SetLocalRotateQ(m_LocalRotation);
}

void TLGameEngine::Transform::LookAt(GameObject* targetObject)
{

}

void TLGameEngine::Transform::LookAt(DirectX::SimpleMath::Vector3 targetPosition)
{
	DirectX::SimpleMath::Vector3 dir = targetPosition - m_worldPosition;

	//dir.Normalize();

	float dis = dir.Length();

	float alpha = acos(dir.x / dis); // 라디안 방향과 right 사이각
	float betha = acos(dir.y / dis); // 라디안 방향과 up 사이각
	float gamma = acos(dir.z / dis); // 라디안 방향과 forward 사이각


	//좌표계에 맞게 변환
	float Angle_Y = -atan2(dir.z, dir.x) + 90 * Deg2Rad;

	DirectX::SimpleMath::Vector3 temp = { 0.0f, Angle_Y, 0.0f };;

	DirectX::SimpleMath::Matrix yRot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(temp);

	DirectX::SimpleMath::Vector3 rotForward = DirectX::SimpleMath::Vector3::Transform({ 0,0,1 }, yRot);

	float xAngle;

	if (Angle_Y < 180 * Deg2Rad && Angle_Y < 90 * Deg2Rad) // 90 < y < 180 => -x -z
	{

	}
	else if (Angle_Y < 180 * Deg2Rad && Angle_Y < 90 * Deg2Rad) // 0 < y < 90 => +x -z
	{

	}
	else if (Angle_Y < 180 * Deg2Rad && Angle_Y < 90 * Deg2Rad) // -90 < y < 0 => +x +z
	{

	}
	else if (Angle_Y < 180 * Deg2Rad && Angle_Y < 90 * Deg2Rad) // -180 < y < -90 => -x +z
	{

	}

	if (dir.y > 0)
	{
		xAngle = -acos(dir.Dot(rotForward) / (dir.Length() * rotForward.Length()));
	}
	else if (dir.y < 0)
	{
		xAngle = acos(dir.Dot(rotForward) / (dir.Length() * rotForward.Length()));
	}
	else
	{
		xAngle = 0;
	}

	//float Angle_X = atan2(dir.y, dir.z);

	m_localEulerRotation = { xAngle, Angle_Y/* + 90 * Deg2Rad*/, 0.0f };

	SetLocalRotate(m_localEulerRotation);
}