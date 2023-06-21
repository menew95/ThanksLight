#pragma once
#include "Component.h"

/// <summary>
/// Light Component
/// 이걸 단 오브젝트는 라이트 효과를 가진다.
/// 엔진에게 cb에게 포지션, 라이트 종류들을 설정
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

		// Directional 말고 다른 빛 일 경우
		float m_Range;			// point / spot
		float m_SpotAngle;		// Spot 일경우 밑면의 각도 Degree
		ParsingColor m_Color;		// 빛의 색
		float m_Intensity;		// 빛의 강도
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