#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class Light;
	class MeshRenderer;
	class Material;
	class Narration;
	class AudioSource;
	class InteractiveObject;
	class Rigidbody;

	class Level2Event :
	    public Component
	{
	public:
		Level2Event();
		~Level2Event();


		virtual void Awake() override;
		virtual void Update() override;

	private:
		void ShootBall();
		void OnOffLight(bool value);

	private:
		bool m_bStart = false;
		bool m_bLightOn = false;
		float m_fTimer = false;
		int m_event = 0;
		int m_maxEvent = 2;
		bool m_endEvent = 0;

		std::weak_ptr<Light> m_light;
		std::weak_ptr<Rigidbody> m_ballGameObject;
		std::weak_ptr<Narration> m_narration;
		std::weak_ptr<InteractiveObject> m_interativeObject;

		//Light
		FileID m_lightID;
		// GameObject
		FileID m_ballID;
		// Narration
		FileID m_narrationID;
		// InteractiveObject
		FileID m_interativeObjectID;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Level2Event, (), (), (), (m_lightID, m_ballID, m_narrationID, m_interativeObjectID))
	};
}