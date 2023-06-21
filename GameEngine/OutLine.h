#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class Mesh;
	class Transfrom;

	class OutLine :
	    public Component
	{
	public:
		OutLine();
		~OutLine();

		virtual void Awake() override;
		virtual void LateUpdate() override;

	private:
		std::shared_ptr<Transform> m_transform;

		std::shared_ptr<Mesh> m_mesh;

		BOOST_DESCRIBE_CLASS(TLGameEngine::OutLine, (), (), (), ())
	};
}

