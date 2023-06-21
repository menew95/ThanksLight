#include "pch.h"
#include "MyTime.h"

using namespace std::chrono;

void TLGameEngine::Time::Init()
{
	m_startTime = steady_clock::now();
	m_preTime = m_startTime;
	m_frameTime = 0.f;
	m_frameCount = 0;
}

void TLGameEngine::Time::Update()
{
	auto now = steady_clock::now();
	duration<double> duration = now - m_preTime;
	m_preTime = now;
	m_deltaTime = duration.count();

	m_frameCount++;
	m_frameTime += m_deltaTime;

	if (m_frameTime > 1.f)
	{
	m_fps = static_cast<size_t>(m_frameCount / m_frameTime);
	m_frameTime = 0;
	m_frameCount = 0;
	}
}

float TLGameEngine::Time::GetDeltaTime()
{
	return static_cast<float>(m_deltaTime);
}

size_t TLGameEngine::Time::GetFps()
{
	return m_fps;
}
