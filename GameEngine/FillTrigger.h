#pragma once
#include "Trigger.h"

namespace TLGameEngine
{
	class GameObject;
	class InteractiveObject;

	class FillTrigger:
		public Trigger
	{
	public:
		FillTrigger();
		virtual ~FillTrigger();

	public:
		// --------- Component Implement ------------------------
		virtual void Awake() override;
		virtual void Update() override;

		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;

		//----------------------------------------------------------

	private:
		bool m_OnceUpdate = false;

		std::weak_ptr<GameObject> m_ForegroundMeshObj;
		std::weak_ptr<InteractiveObject> m_CoreectObj;


	private:
		//GameObject
		FileID m_ForegroundMesh;
		FileID m_ForegroundTrigger;
		FileInfo m_KeyObject;
		FileInfo m_KeyType;

		BOOST_DESCRIBE_CLASS(TLGameEngine::FillTrigger, (), (), (), (m_ForegroundMesh, m_ForegroundTrigger, m_KeyObject, m_KeyType))
	};
}



