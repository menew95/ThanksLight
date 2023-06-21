#pragma once
#include "Trigger.h"

namespace TLGameEngine
{
	class Door;
	class Material;
	class MeshRenderer;

	class DoorTrigger :
		public Trigger
	{
	public:
		DoorTrigger();
		virtual ~DoorTrigger();

	public:
		// --------- Component Implement ------------
		virtual void Awake() override;

		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;

		//-----------------------

	private:
		//GameObject
		FileID m_PlayerID;

		BOOST_DESCRIBE_CLASS(TLGameEngine::DoorTrigger, (), (), (), (m_PlayerID))
	};

}