#pragma once
#include "Trigger.h"

//3D가 되었을떄 콜라이더 활성화하자..

namespace TLGameEngine
{
	class Collider;
	class InteractiveObject;

	class Level5ButtonTrigger :
		public Component
	{
	public:
		Level5ButtonTrigger();
		virtual ~Level5ButtonTrigger();

	public:
		// --------- Component Implement ------------------------
		virtual void Awake() override;
		virtual void Update() override;


		//----------------------------------------------------------

	private:
		std::weak_ptr<InteractiveObject> m_Button;
		std::weak_ptr<Collider> m_Collider;


	private:
		//GameObject
		FileID m_ButtonTrigger;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Level5ButtonTrigger, (), (), (), (m_ButtonTrigger))
	};
}
