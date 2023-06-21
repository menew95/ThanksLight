#pragma once
#include "Component.h"

namespace TLGameEngine
{
	class InteractiveObject;

	class Level6DoorEvent :
		public Component
	{
	public:
		Level6DoorEvent();
		virtual ~Level6DoorEvent();

	public:
		// --------- Component Implement ------------------------
		virtual void Awake() override;
		virtual void Update() override;

		//----------------------------------------------------------

	private:
		std::weak_ptr<InteractiveObject> m_Object;

	private:
		//GameObject	
		BOOST_DESCRIBE_CLASS(TLGameEngine::Level6DoorEvent, (), (), (), ())
	};
}
