#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class PostProcess :
	    public Component
	{
	public:
		PostProcess();
		~PostProcess();

	public:
		virtual void Awake();

		virtual void SetBlur(bool value);
		virtual void SetBloom(bool value);
		virtual void SetBloomThreshold(float min, float max);

		virtual void SetToneMap(bool value);
		virtual void SetToneMapExposure(float value);
		virtual void SetIBLFactor(float value);

	public:
		int m_onOffBloom = false;
		int m_onOffTone = false;
		float m_min = 1.0f;
		float m_max = 1.26f;
		float m_exposure = 2.0f;
		float m_IBLFactor = 0.0f;

		BOOST_DESCRIBE_CLASS(TLGameEngine::PostProcess, (), (m_onOffBloom, m_onOffTone, m_min, m_max, m_exposure, m_IBLFactor), (), ())
	};
}