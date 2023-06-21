#pragma once
#include "Collider.h"

namespace TLGameEngine
{
	class SphereCollider :
		public Collider
	{
	public:
		SphereCollider();
		SphereCollider(Vector3 center, float radius);
		virtual ~SphereCollider();

	public:

		float GetRaduis() const { return m_Radius; }
		void SetRaduis(float val) { m_Radius = val; }

	protected:
		float m_Radius = 1.0f;
		BOOST_DESCRIBE_CLASS(TLGameEngine::SphereCollider, (), (), (), (m_Center, m_Radius, m_IsTrigger))
	};
}