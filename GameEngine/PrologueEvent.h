#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class Light;
	class MeshRenderer;
	class Material;
	class Narration;
	class AudioSource;

	class PrologueEvent :
	    public Component
	{
	public:
		PrologueEvent();
		~PrologueEvent();

		
		virtual void Awake() override;
		virtual void Update() override;

	private:
		bool m_bStart = false;
		bool m_bLightOn = false;
		float m_fTimer = 0.0f;
		int m_ilightOnCount = 0.0f;
		std::weak_ptr<Light> m_light1;
		std::weak_ptr<Light> m_light2;
		std::weak_ptr<Light> m_light3;
		std::weak_ptr<Light> m_light4;
		std::weak_ptr<Light> m_light5;
		std::weak_ptr<Light> m_light6;
		std::weak_ptr<MeshRenderer> m_lightEmmisive1;
		std::weak_ptr<MeshRenderer> m_lightEmmisive2;
		std::weak_ptr<MeshRenderer> m_lightEmmisive3;
		std::weak_ptr<MeshRenderer> m_lightEmmisive4;
		std::shared_ptr<Material> m_emmisiveMaterial;
		std::weak_ptr<Narration> m_narration;
		std::weak_ptr<AudioSource> m_audioSource;

		//Light
		FileID m_light1ID;
		FileID m_light2ID;
		FileID m_light3ID;
		FileID m_light4ID;
		FileID m_light5ID;
		FileID m_light6ID;
		// MeshRenderer
		FileID m_lightEmmisive1ID;
		FileID m_lightEmmisive2ID;
		FileID m_lightEmmisive3ID;
		FileID m_lightEmmisive4ID;

		FileInfo m_emmisiveMaterialID;
		FileInfo m_lightClipID;
		BOOST_DESCRIBE_CLASS(TLGameEngine::PrologueEvent, (), (), (), (m_light1ID, m_light2ID, m_light3ID, m_light4ID, m_light5ID, m_light6ID, m_lightEmmisive1ID, m_lightEmmisive2ID, m_lightEmmisive3ID, m_lightEmmisive4ID, m_emmisiveMaterialID, m_lightClipID))
	};
}