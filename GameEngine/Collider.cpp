#include "pch.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "PhysicsManager.h"
#include "Rigidbody.h"

TLGameEngine::Collider::Collider() : 
	Component(Type::Physics),
	m_IsTrigger(false),
	m_ColliderLayer("Default")
	//m_RigidBody(nullptr),
	//m_Transform(nullptr)
{
}

TLGameEngine::Collider::Collider(DirectX::SimpleMath::Vector3 center) :
	Component(Type::Physics),
	m_Center(center),
	m_IsTrigger(false),
	m_ColliderLayer("Default")
	//m_RigidBody(nullptr),
	//m_Transform(nullptr)
{

}

TLGameEngine::Collider::~Collider()
{

}

void TLGameEngine::Collider::AddTriggerEvent(PhysicsEngine::IEventTrigger* callBackClass)
{
	PhysicsManager::Instance().AddTriggerEvent(this, callBackClass);
}

void TLGameEngine::Collider::Awake()
{

	m_Transform = m_gameObject.lock()->GetComponent<Transform>();
	m_RigidBody = m_gameObject.lock()->GetComponent<Rigidbody>();

	if (!m_RigidBody.lock())
	{
		m_physicsType = PhysicsEngine::ePhysicsType::eSTATTIC;
	}
	else
	{
		m_physicsType = PhysicsEngine::ePhysicsType::eDYNAMIC;
	}

	PhysicsManager::Instance().AddObject(this);

	if (m_RigidBody.lock())
	{
		m_RigidBody.lock()->UpdateRigidBodySetting();
	}
	PhysicsManager::Instance().SetColliderLayer(this, m_ColliderLayer);
}

void TLGameEngine::Collider::OnEnable()
{
	PhysicsManager::Instance().EnableObject(this);

}

void TLGameEngine::Collider::OnDisalbe()
{
	PhysicsManager::Instance().DisableObject(this);
}

void TLGameEngine::Collider::Start()
{

}

void TLGameEngine::Collider::Update()
{
	PhysicsManager::Instance().UpdateTransform(this);

}

void TLGameEngine::Collider::LateUpdate()
{

}

void TLGameEngine::Collider::FixedUpdate()
{
	auto worldPos = m_Transform.lock().get()->GetWorldPosition();
	auto worldQuat = m_Transform.lock().get()->GetWorldRotateQ();

	PhysicsEngine::Transform transform;

	transform.quat[0] = worldQuat.x;
	transform.quat[1] = worldQuat.y;
	transform.quat[2] = worldQuat.z;
	transform.quat[3] = worldQuat.w;

	transform.vec[0] = worldPos.x;
	transform.vec[1] = worldPos.y;
	transform.vec[2] = worldPos.z;

	PhysicsManager::Instance().SetTransform(this, transform);
}

void TLGameEngine::Collider::OnDestroy()
{

}

void TLGameEngine::Collider::SetColliderLayer(const std::string& val)
{
	m_ColliderLayer = val;
	PhysicsManager::Instance().SetColliderLayer(this, val);
}


