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
	// �׳� GetComponent �ϴ°� ��������?
	m_light = SceneManager::Instance().FindObject(m_lightID)->GetComponent<Light>();

	m_vantaMaterial = ResourceManager::Instance().GetMaterial(m_vantaMaterialID.guid);
}

void TLGameEngine::FlashLight::OnOffLight(bool value)
{
	m_light.lock()->SetIsActive(value);

	struct IAMGUILTY
	{
		Vector3	GUILTY1;	// �̰��� ī�޶� �������� ����
		float	GUILTY2;	// ������ ���� �ޱ� ����
		int	GUILTY3[4];		// ����Ʈ �¿��� ����
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
		// ��Ÿ ���׸��� ���� X
		assert(false);
	}
}
