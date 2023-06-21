#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class Narration;
	class Door;
	class InteractiveObject;
	class Rigidbody;

	class Level7Event :
	    public Component
	{
	public:
		Level7Event();
		~Level7Event();

		virtual void Awake() override;
		virtual void Update() override;

	private:
		bool m_bEndEvnet = false;
		int m_event = false;
		
		std::weak_ptr<Door> m_door;
		std::weak_ptr<Narration> m_narration1;
		std::weak_ptr<Narration> m_narration2;
		std::weak_ptr<Rigidbody> m_interactive;

		FileID m_doorID;
		FileID m_narrationID;
		FileID m_interactiveID;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Level7Event, (), (), (), (m_doorID, m_narrationID, m_interactiveID))
	};
}

