#pragma once
#include "Component.h"

/// <summary>
/// Light Component
/// �̰� �� ������Ʈ�� ����Ʈ ȿ���� ������.
/// �������� cb���� ������, ����Ʈ �������� ����
/// </summary>
namespace TLGameEngine
{
	class Light :
		public Component
	{
	public:
		Light();
		virtual ~Light();

	public:
		enum class LightType
		{
			Spot = 0,
			Directional = 1,
			Point = 2,
			BlackSpotLight = 3
		};

		// Directional ���� �ٸ� �� �� ���
		float m_Range;			// point / spot
		float m_SpotAngle;		// Spot �ϰ�� �ظ��� ���� Degree
		ParsingColor m_Color;		// ���� ��
		float m_Intensity;		// ���� ����
		float m_fFallOff;		// point / spot
		float m_InnerSpotAngle;	// spot

		int m_iLightID;

	public:
		virtual void Init(GameObject* gameObject);

		virtual void Awake()			override;
		virtual void OnEnable()			override;
		virtual void OnDisalbe()		override;
		virtual void Update()			override;
		virtual void LateUpdate()		override;

		virtual void OnDestroy()		override;

		void SetLightType(LightType type) { m_Type = (int)type; }
	private:
		int m_Type;
		BOOST_DESCRIBE_CLASS(TLGameEngine::Light, (), ( m_Range, m_SpotAngle, m_Color, m_Intensity, m_InnerSpotAngle), (), (m_Type))
	};
}