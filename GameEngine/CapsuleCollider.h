#pragma once
#include "Collider.h"

namespace TLGameEngine
{
	class CapsuleCollider :
		public Collider
	{
	public:
		CapsuleCollider();
		CapsuleCollider(Vector3 center, float radius, float halfHeight);
		virtual ~CapsuleCollider();

	public:


		float GetRaduis() const { return m_Radius; }
		void SetRaduis(float val) { m_Radius = val; }
		float GetHeight() const { return m_Height; }
		void SetHeight(float val) { m_Height = val; }

	protected:
		float m_Radius;
		float m_Height;

		BOOST_DESCRIBE_CLASS(TLGameEngine::CapsuleCollider, (), (), (), (m_Center, m_Height, m_Radius, m_IsTrigger))
	};
}