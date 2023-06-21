#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class Narration;
	class AudioSource;
	class InteractiveObject;

	class Level3Event :
	    public Component
	{
	public:
		Level3Event();
		~Level3Event();

		virtual void Awake() override;
		virtual void Update() override;

	private:
		bool m_bStart = false;
		bool m_bLightOn = false;
		float m_fTimer = false;
		int m_event = 0;
		int m_maxEvent = 2;
		bool m_endEvent = 0;

		std::weak_ptr<Narration> m_narration1;
		std::weak_ptr<Narration> m_narration2;
		std::weak_ptr<InteractiveObject> m_interativeObject;

		FileID m_narration1ID;
		FileID m_narration2ID;
		FileID m_interativeObjectID;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Level3Event, (), (), (), (m_narration1ID, m_narration2ID, m_interativeObjectID))
	};
}

