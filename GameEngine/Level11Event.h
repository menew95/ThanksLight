#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class Narration;
	class InteractiveObject;
	class Transform;
	class TableSnapTrigger;

	class Level11Event :
		public Component
	{
	public:
		Level11Event();
		~Level11Event();


		virtual void Awake() override;
		virtual void Update() override;

		void UpdateBildBordSize();
	private:
		bool m_bEndEvnet = false;
		int m_event = 0;
		int m_count = 0;
		float m_timer = 0;
		bool m_timerSet = false;
		bool m_isSetting = false;
		std::weak_ptr<Narration> m_narration;
		std::weak_ptr<Transform> m_spotLight;
		std::weak_ptr<Transform> m_interactive;
		std::vector<std::weak_ptr<InteractiveObject>> m_bildbords;
		std::weak_ptr<TableSnapTrigger> m_trigger;

		FileID m_narrationID;
		FileID m_spotLightID;
		FileID m_interactiveID;
		std::vector<FileID> m_bildbordIDs;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Level11Event, (), (), (), (m_narrationID, m_spotLightID, m_interactiveID, m_bildbordIDs))
	};
}

