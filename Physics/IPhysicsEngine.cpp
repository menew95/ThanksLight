#include "pch.h"
#include "Export/IPhysicsEngine.h"
#include "PhsXPhysicsEngine.h"

PHYSICS_ENGINE std::shared_ptr<PhysicsEngine::IPhysicsEngine> PhysicsEngine::PhysicsEngineInstance()
{
	if (PhysicsEngine::g_physicsEngine == nullptr)
	{
		g_physicsEngine = std::make_shared<PhysicsEngine::PhsXPhysicsEngine>();
		return g_physicsEngine;
	}
	else
	{
		return g_physicsEngine;
	}

}

PHYSICS_ENGINE void PhysicsEngine::PhysicsEngineRelease()
{
	g_physicsEngine.reset();
}

