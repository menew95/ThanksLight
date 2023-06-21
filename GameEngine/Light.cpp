#include "pch.h"
#include "Light.h"
#include "GameEngine.h"
#include "IGraphicsEngine.h"
#include "Transform.h"

TLGameEngine::Light::Light()
: Component(Type::Rendering)
, m_Range(20.0f)
, m_SpotAngle(0.0f)
, m_Color(1.0f, 1.0f, 1.0f, 1.0f)
, m_Intensity(1.0f)
, m_fFallOff(0.0f)
, m_InnerSpotAngle(0.0f)
, m_iLightID(0)
{
}

TLGameEngine::Light::~Light()
{
	//GameEngine::Instance().GetGraphicsEngine()->DeleteLight(m_iLightID);
}

void TLGameEngine::Light::Init(GameObject* gameObject)
{
	//Component::Init(gameObject);
}


void TLGameEngine::Light::Awake()
{
	m_iLightID = GameEngine::Instance().GetGraphicsEngine()->AddLight();
}

void TLGameEngine::Light::OnEnable()
{
	GameEngine::Instance().GetGraphicsEngine()->SetLightEnable(m_iLightID, true);
}

void TLGameEngine::Light::OnDisalbe()
{
	GameEngine::Instance().GetGraphicsEngine()->SetLightEnable(m_iLightID, false);
}

void TLGameEngine::Light::Update()
{
	GameEngine::Instance().GetGraphicsEngine()->SetLightEnable(m_iLightID, true);

	//if (!m_Enable)
	//{
	//	return;
	//}

	std::shared_ptr<Transform> transform = GetTransform();

	DirectX::SimpleMath::Vector3 position = transform->GetWorldPosition();

	DirectX::SimpleMath::Vector3 direction = transform->GetForward();


	GameEngine::Instance().GetGraphicsEngine()->UpdateLight(m_iLightID, static_cast<int>(m_Type), m_Range
		, m_fFallOff, m_SpotAngle * Deg2Rad, m_SpotAngle * 0.5f * Deg2Rad, m_Intensity, &position.x, &direction.x, &m_Color.r);
}

void TLGameEngine::Light::LateUpdate()
{
}

void TLGameEngine::Light::OnDestroy()
{
	//GameEngine::m_pGraphicEngine->DeleteLight(m_iLightID);
}