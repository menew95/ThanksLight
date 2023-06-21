#pragma once
#include "Trigger.h"

//3D�� �Ǿ����� �ݶ��̴� Ȱ��ȭ����..

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
