#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class ProjectedCtrlTest :
	    public Component
	{
	public:
		ProjectedCtrlTest();
		~ProjectedCtrlTest();

	public:
		void Awake();
		void Update();

	private:
		std::weak_ptr<GameObject> m_projectedObject;

		FileID m_projectedObjectID;

		BOOST_DESCRIBE_CLASS(TLGameEngine::ProjectedCtrlTest, (), (), (), (m_projectedObjectID))
	};
}

