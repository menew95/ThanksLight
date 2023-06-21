#include "pch.h"
#include "Shape.h"

PhysicsEngine::Shape::Shape(physx::PxShape* shape, float density /*= 1.f*/)
{
	m_shape = shape;
	m_density = density;
}

PhysicsEngine::Shape::~Shape()
{
	m_shape->release();
	m_shape = nullptr;
}
