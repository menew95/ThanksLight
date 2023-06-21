#pragma once
#include "Trigger.h"

namespace TLGameEngine
{
	class Door;
	class Material;
	class MeshRenderer;
	class InteractiveObject;

	class ButtonTrigger:
		public Trigger
	{
	public:
		ButtonTrigger();
		virtual ~ButtonTrigger();

	public:
		// --------- Component Implement ------------
		virtual void Awake() override;
		virtual void Start() override;

		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;

		//-----------------------
		void SetForceTriggerEvent(bool value);

	private:
		std::weak_ptr<InteractiveObject> m_interactiveObject;
		std::weak_ptr<MeshRenderer> m_buttonMeshRenderer;
		std::shared_ptr<Material> m_openEmissive;
		std::shared_ptr<Material> m_closeEmissive;


		//GameObject
		FileID m_PrimitiveID;
		FileID m_buttonObjectID;
		FileInfo m_openEmissiveID;
		FileInfo m_closeEmissiveID;
		
		BOOST_DESCRIBE_CLASS(TLGameEngine::ButtonTrigger, (), (), (), (m_PrimitiveID, m_buttonObjectID, m_openEmissiveID, m_closeEmissiveID))
	};

}

