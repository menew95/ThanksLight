#pragma once

/// <summary>
/// PxShape DataHolder
/// </summary>

namespace PhysicsEngine
{
	class Shape
	{
	public:
		Shape(physx::PxShape* shape, float density = 1.f);
		~Shape();

	public:
		
		physx::PxShape* GetShape() const { return m_shape; }
		void SetShape(physx::PxShape* val) { m_shape = val; }

		float GetDensity() const { return m_density; }
		void SetDensity(float val) { m_density = val; }
	
	private:
		Shape();

		physx::PxShape* m_shape;
		float m_density;

	};

}
