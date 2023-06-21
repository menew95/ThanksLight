#pragma once
#include "Trigger.h"

namespace TLGameEngine
{
	class Image;

	class NextSceneTrigger
		: public Trigger
	{
	public:
		NextSceneTrigger();
		virtual ~NextSceneTrigger();

	public:
		// --------- Component Implement ------------
		virtual void Awake() override;
		virtual void Update() override;

		void FadeOut();
		void FadeIn();

		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;

		//-----------------------

	private:
		bool m_bCallNextScene = false;
		bool m_bFadeIn = true;
		float m_fTimer = 3.0f;

		std::weak_ptr<Image> m_fadeOut;

		//GameObject
		FileID m_playerID;
		FileID m_fadeOutID;
		std::string m_nextSceneID;
		BOOST_DESCRIBE_CLASS(TLGameEngine::NextSceneTrigger, (), (), (), (m_playerID, m_fadeOutID, m_nextSceneID))
	};
}
