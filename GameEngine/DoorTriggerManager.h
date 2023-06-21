#pragma once
#include "Component.h"
#include "..\Physics\Export\PhysicsInfo.h"
#include "Trigger.h"

namespace TLGameEngine
{
	class Door;

	class DoorTriggerManager :
		public Trigger
	{
	public:
		DoorTriggerManager();
		virtual ~DoorTriggerManager();

	public:
		// --------- Component Implement ------------
		virtual void Awake() override;
		virtual void Update() override;

		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;

		//-----------------------


	private:
		//GameObject
		std::weak_ptr<Door> m_Door;
		std::vector<std::weak_ptr<Trigger>> m_Triggers;

		bool m_TriggersResult;

	private:
		//GameObject
		FileID m_DoorID;

		int m_nbOfTriggers;

		FileID m_Trigger1;
		FileID m_Trigger2;
		FileID m_Trigger3;
		FileID m_Trigger4;
		FileID m_Trigger5;

		BOOST_DESCRIBE_CLASS(TLGameEngine::DoorTriggerManager, (), (), (), (m_DoorID, m_nbOfTriggers, m_Trigger1, m_Trigger2, m_Trigger3, m_Trigger4, m_Trigger5))
	};

}

