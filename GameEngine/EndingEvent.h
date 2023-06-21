#pragma once

#include "Component.h"

namespace TLGameEngine
{
class Image;
class Narration;

class EndingEvent : public Component
{
public:
	EndingEvent();

	virtual void Awake() override;
	virtual void Update() override;

private:
	std::vector<std::shared_ptr<GameObject>> m_images;

	float m_currentTime = 0;
	int m_currentIndex = 0;
	bool m_endEvent = false;
	std::shared_ptr<Narration> m_narration;

	std::vector<FileID> m_Images;
	FileID m_Narration;
	float m_ImageTransitionIntervalTime;

	BOOST_DESCRIBE_CLASS(TLGameEngine::EndingEvent, (), (), (), (m_Images, m_Narration, m_ImageTransitionIntervalTime))
};
}

