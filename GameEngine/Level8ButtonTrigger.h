#pragma once
#include "Trigger.h"

namespace TLGameEngine
{
	class Door;
	class Material;
	class MeshRenderer;
	class InteractiveObject;

	class Level8ButtonTrigger :
	    public Trigger
	{
	public:
		Level8ButtonTrigger();
		virtual ~Level8ButtonTrigger();

	public:
		// --------- Component Implement ------------
		virtual void Awake() override;\

		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;

	private:
		int m_iNum = 0;
		std::weak_ptr<InteractiveObject> m_interactiveObject;
		std::weak_ptr<MeshRenderer> m_buttonMeshRenderer;
		std::shared_ptr<Material> m_openEmissive;
		std::shared_ptr<Material> m_closeEmissive;

		FileID m_playerID;
		FileID m_PrimitiveID;
		FileID m_buttonObjectID;
		FileInfo m_openEmissiveID;
		FileInfo m_closeEmissiveID;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Level8ButtonTrigger, (), (), (), (m_playerID, m_PrimitiveID, m_buttonObjectID, m_openEmissiveID, m_closeEmissiveID))
	};
}

