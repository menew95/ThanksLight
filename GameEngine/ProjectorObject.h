#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class BoxCollider;
	class ProjectedRenderer;

	class ProjectorObject :
	    public Component
	{
	public:
		ProjectorObject();
		~ProjectorObject();

	public:
		void Awake();
		void Set2D();

		bool GetProjectorOn() { return m_projectorOn; }
	private:
		bool m_projectorOn = false;
		std::weak_ptr<BoxCollider> m_collider;
		std::weak_ptr<ProjectedRenderer> m_projectedRenderer;

		FileID m_primitiveObjectID;

		BOOST_DESCRIBE_CLASS(TLGameEngine::ProjectorObject, (), (), (), (m_primitiveObjectID))
	};
}