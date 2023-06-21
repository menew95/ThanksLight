#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class Narration;
	class InteractiveObject;
	class Transform;

	class Level10Event :
	    public Component
	{
	public:
		Level10Event();
		~Level10Event();

		virtual void Awake() override;
		virtual void Update() override;

	private:
		bool m_bEndEvnet = false;
		int m_event = false;
		bool m_bCheck2D = false;
		std::weak_ptr<Narration> m_narration;

		std::weak_ptr<Transform> m_camera;
		std::weak_ptr<Transform> m_object;
		std::weak_ptr<InteractiveObject> m_interactive1;
		std::weak_ptr<InteractiveObject> m_interactive2;
		std::weak_ptr<InteractiveObject> m_interactive3;

		FileID m_cameraID;
		FileID m_objectID;
		FileID m_interactive1ID;
		FileID m_interactive2ID;
		FileID m_interactive3ID;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Level10Event, (), (), (), (m_cameraID, m_objectID, m_interactive1ID, m_interactive2ID, m_interactive3ID))
	};
}

