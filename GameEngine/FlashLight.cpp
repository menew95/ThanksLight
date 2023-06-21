#include "pch.h"
#include "FlashLight.h"
#include "SceneManager.h"
#include "Light.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Transform.h"
#include "Input.h"
TLGameEngine::FlashLight::FlashLight()
	: Component(Type::Action)
{

}

TLGameEngine::FlashLight::~FlashLight()
{

}

void TLGameEngine::FlashLight::Awake()
{
	// 그냥 GetComponent 하는게 나을지도?
	m_light = SceneManager::Instance().FindObject(m_lightID)->GetComponent<Light>();

	m_vantaMaterial = ResourceManager::Instance().GetMaterial(m_vantaMaterialID.guid);
}

void TLGameEngine::FlashLight::OnOffLight(bool value)
{
	m_light.lock()->SetIsActive(value);

	struct IAMGUILTY
	{
		Vector3	GUILTY1;	// 이곳은 카메라 포지션이 들어갈곳
		float	GUILTY2;	// 손전등 스팟 앵글 각도
		int	GUILTY3[4];		// 라이트 온오프 정보
	};
	IAMGUILTY _iamGuilty;

	_iamGuilty.GUILTY1 = m_light.lock()->GetComponent<Transform>()->GetForward();
	_iamGuilty.GUILTY2 = cos(m_light.lock()->m_SpotAngle / 2 * TLGameEngine::Deg2Rad);
	_iamGuilty.GUILTY3[0] = value;

	if (value == true);
	{
		if (Input::Instance().GetKey(vk_alpha('v')))
		{
			m_light.lock()->SetLightType(Light::LightType::BlackSpotLight);
		}
		else
		{
			m_light.lock()->SetLightType(Light::LightType::Spot);
		}
	}

	if (!m_vantaMaterial.expired())
	{
		m_vantaMaterial.lock()->SetCustomBuffer(&_iamGuilty, sizeof(IAMGUILTY));
	}
	else
	{
		// 반타 머테리얼 설정 X
		assert(false);
	}
}
