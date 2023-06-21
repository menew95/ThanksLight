#pragma once
#include "Trigger.h"

namespace TLGameEngine
{
	class GameObject;
	class Material;
	class MeshRenderer;
	class InteractiveObject;

	class ButtonTrigger2 :
		public Trigger
	{
	public:
		ButtonTrigger2();
		virtual ~ButtonTrigger2();

	public:
		// --------- Component Implement ------------------------
		virtual void Awake() override;
		virtual void Update() override;

		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerPersist(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;

		//----------------------------------------------------------

	private:

		std::weak_ptr<MeshRenderer> m_buttonMeshRenderer;

		std::shared_ptr<Material> m_openEmissive;
		std::shared_ptr<Material> m_closeEmissive;

		std::queue<bool> m_CurrentTriggerEnterObject;

		bool m_defaultState;

	private:
		//GameObject
		FileInfo m_KeyObject;

		FileID m_buttonObjectID;
		FileInfo m_openEmissiveID;
		FileInfo m_closeEmissiveID;
		int m_SetState;

		BOOST_DESCRIBE_CLASS(TLGameEngine::ButtonTrigger2, (), (), (), (m_KeyObject, m_buttonObjectID, m_openEmissiveID, m_closeEmissiveID, m_SetState))
	};
}


