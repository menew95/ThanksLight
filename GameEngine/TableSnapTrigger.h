#pragma once
#include "Trigger.h"

namespace TLGameEngine
{
	class Door;
	class Material;
	class InteractiveObject;

	class TableSnapTrigger :
		public Trigger
	{
	public:
		TableSnapTrigger();
		virtual ~TableSnapTrigger();

	public:
		// --------- Component Implement ------------
		virtual void Awake() override;
		virtual void Update() override;

		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;

		//-----------------------

		void SnapObject();
		bool GetIsSnap() { return m_bIsSnap; }
		void OnOff(bool value) { m_bOnOff = value; }
	private:
		bool m_bIsSnap = false;
		bool m_bOnOff = true;
		std::weak_ptr<InteractiveObject> m_interactive;
		std::weak_ptr<Transform> m_interactiveTransform;

		//GameObject
		FileID m_interactiveID;

		BOOST_DESCRIBE_CLASS(TLGameEngine::TableSnapTrigger, (), (), (), (m_interactiveID))
	};

}