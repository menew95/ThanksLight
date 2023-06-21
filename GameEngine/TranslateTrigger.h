#pragma once
#include "Trigger.h"

namespace TLGameEngine
{
	class Transform;
	class InteractiveObject;

	class TranslateTrigger:
		public Trigger
	{
	public:
		TranslateTrigger();
		virtual ~TranslateTrigger();

	public:
		// --------- Component Implement ------------------------
		virtual void Awake() override;
		virtual void Update() override;

		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) {};

		//----------------------------------------------------------

	private:
		std::queue<std::weak_ptr<Transform>> m_TranslateObjects;

	private:
		//GameObject
		float m_worldTranslatePos_X;
		float m_worldTranslatePos_Y;
		float m_worldTranslatePos_Z;

		BOOST_DESCRIBE_CLASS(TLGameEngine::TranslateTrigger, (), (), (), (m_worldTranslatePos_X, m_worldTranslatePos_Y, m_worldTranslatePos_Z))
	};

}