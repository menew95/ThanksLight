#pragma once
#include "Collider.h"

namespace TLGameEngine
{
	class BoxCollider :
	    public Collider
	{
	public:
		BoxCollider();
		BoxCollider(Vector3 center, Vector3 size);

		virtual ~BoxCollider();

	public:
		virtual void OnDrawGizmo();

		Vector3 GetSize() const { return m_Size; }
		void SetSize(Vector3 val) { m_Size = val; }

	protected:
		Vector3 m_Size = Vector3::One;

		BOOST_DESCRIBE_CLASS(TLGameEngine::BoxCollider, (), (), (), (m_Center, m_Size, m_IsTrigger))
	};
}