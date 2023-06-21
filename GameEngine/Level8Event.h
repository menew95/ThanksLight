#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class ProjectorObject;
	class Narration;
	class InteractiveObject;
	class ButtonTrigger;

	class Level8Event :
	    public Component
	{
	public:
		Level8Event();
		~Level8Event();
	
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
	
	private:
		int m_event = 0;
		bool m_bEndEvent = false;
		bool closeDoor = false;
		std::vector<std::weak_ptr<ProjectorObject>> m_projectors;
		std::weak_ptr<Narration> m_narration1;
		std::weak_ptr<Narration> m_narration2;
		std::weak_ptr<GameObject> m_interactive;
		std::weak_ptr<ButtonTrigger> m_button;
	
		std::vector<FileID> m_projectorIDs;
		FileID m_narrationID;
		FileID m_interactiveID;
		FileID m_buttonID;
	
		BOOST_DESCRIBE_CLASS(TLGameEngine::Level8Event, (), (), (), (m_projectorIDs, m_narrationID, m_interactiveID, m_buttonID))
	};
}

