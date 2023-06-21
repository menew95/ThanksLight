#pragma once

#include "..\Common\Singleton.h"
#include <chrono>

namespace TLGameEngine
{
class Time : public Singleton<Time>
{
public:
	void Init();
	void Update();

	float GetDeltaTime();
	size_t GetFps();

private:
	size_t m_frameCount;
	std::chrono::steady_clock::time_point m_startTime;
	std::chrono::steady_clock::time_point m_preTime;
	double m_deltaTime;
	double m_frameTime;
	size_t m_fps;

private:
	Time() = default;
	friend Singleton;
};
}
