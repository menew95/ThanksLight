#include "pch.h"
#include "PostProcess.h"
#include "GameEngine.h"
#include "../GraphicsEngine/Export/IGraphicsEngine.h"

TLGameEngine::PostProcess::PostProcess()
	: Component(Type::Action)
{

}

TLGameEngine::PostProcess::~PostProcess()
{

}

void TLGameEngine::PostProcess::Awake()
{
	SetBloom(m_onOffBloom);
	SetBloomThreshold(m_min, m_max);
	SetToneMap(m_onOffTone);
	SetToneMapExposure(m_exposure);
	SetIBLFactor(m_IBLFactor);
}

void TLGameEngine::PostProcess::SetBlur(bool value)
{
	GameEngine::Instance().GetGraphicsEngine()->SetBlur(value);
}

void TLGameEngine::PostProcess::SetBloom(bool value)
{
	{
		m_onOffBloom = value;
	}
	GameEngine::Instance().GetGraphicsEngine()->SetBloom(value);
}

void TLGameEngine::PostProcess::SetBloomThreshold(float min, float max)
{
	{
		m_min = min;
		m_max = max;
	}
	GameEngine::Instance().GetGraphicsEngine()->SetBloomThreshold(min, max);
}

void TLGameEngine::PostProcess::SetToneMap(bool value)
{
	{
		m_onOffTone = value;
	}
	GameEngine::Instance().GetGraphicsEngine()->SetToneMap(value);
}

void TLGameEngine::PostProcess::SetToneMapExposure(float value)
{
	{
		m_exposure = value;
	}
	GameEngine::Instance().GetGraphicsEngine()->SetToneMapExposure(value);
}

void TLGameEngine::PostProcess::SetIBLFactor(float value)
{
	{
		m_IBLFactor = value;
	}
	GameEngine::Instance().GetGraphicsEngine()->SetIBLFactor(m_IBLFactor);
}
