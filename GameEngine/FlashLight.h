#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class Material;
	class Light;

	class FlashLight :
	    public Component
	{
	public:
		FlashLight();
		~FlashLight();

	public:
		virtual void Awake() override;
		void OnOffLight(bool value);

	private:
		std::weak_ptr<Material> m_vantaMaterial;
		std::weak_ptr<Light> m_light;

		FileInfo m_vantaMaterialID;
		FileID m_lightID;
		BOOST_DESCRIBE_CLASS(TLGameEngine::FlashLight, (), (), (), (m_vantaMaterialID, m_lightID))
	};
}