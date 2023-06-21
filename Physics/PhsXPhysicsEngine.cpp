#include "pch.h"
#include "PhsXPhysicsEngine.h"
#include "Scene.h"

PhysicsEngine::PhsXPhysicsEngine::PhsXPhysicsEngine()
{

}

PhysicsEngine::PhsXPhysicsEngine::~PhsXPhysicsEngine()
{

}

bool PhysicsEngine::PhsXPhysicsEngine::Initialize()
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_defaultAllocatorCallback, m_defaultErrorCallback);
	if (!m_foundation)
		PX_ASSERT(false);

	bool recordMemoryAllocations = true;
	//m_toleranceScale = physx::PxTolerancesScale();
	m_toleranceScale.length = 100;
	m_toleranceScale.speed = 9.81f;

#if defined(DEBUG) || defined(_DEBUG)
	//Physics Visual Debugger
	m_pvd = PxCreatePvd(*m_foundation);
	m_pvdTransport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	m_pvd->connect(*m_pvdTransport, physx::PxPvdInstrumentationFlag::eALL);

	if (!m_pvd)
	{
		//pvd연결실패
	}
#endif

	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, m_toleranceScale, true, m_pvd);

	//스레딩관련 
	m_cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);

	//TriangleMesh를 위한 쿠킹 라이브러리

	

	if (!m_physics)
	{
		assert(false);
		return false;
	}

	m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, physx::PxCookingParams(m_toleranceScale));
	//m_physics->registerDeletionListener(*this, physx::PxDeletionEventFlag::eUSER_RELEASE);
	return true;
}

bool PhysicsEngine::PhsXPhysicsEngine::Finalalize()
{

	for (auto scene : m_scenes)
	{
		scene.second->Finalize();
		scene.second.reset();
	}

	m_currentScene.reset();
	m_cooking->release();

#if defined(DEBUG) || defined(_DEBUG)
	m_pvd->disconnect();
	m_pvdTransport->disconnect();
	m_pvdTransport->release();
#endif

	m_cpuDispatcher->release();
	m_physics->release();

#if defined(DEBUG) || defined(_DEBUG)
	m_pvd->release();
#endif

	m_foundation->release();
	return true;
}

bool PhysicsEngine::PhsXPhysicsEngine::AddScene(const std::string& sceneName)
{
	auto foundedScene = m_scenes.find(sceneName);
	if (foundedScene == m_scenes.end())
	{
		auto tempScene = std::make_shared<Scene>();
		tempScene->Initialize(m_physics, m_cpuDispatcher, m_toleranceScale);
		m_scenes.emplace(sceneName, tempScene);
		return true;
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetCurrentScene(const std::string& sceneName)
{
	auto foundedScene = m_scenes.find(sceneName);
	if (foundedScene == m_scenes.end())
	{
		return false;
	}
	else
	{
		m_currentScene = foundedScene->second;
		return true;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::DeleteScene(const std::string& sceneName)
{
	auto foundedScene = m_scenes.find(sceneName);
	if (foundedScene == m_scenes.end())
	{
		return false;
	}
	else
	{
		foundedScene->second->Finalize();
		m_scenes.erase(sceneName);
		return true;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::CreateBoxActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, Vector3 halfExtend, float density)
{
	if (m_currentScene)
	{
		m_currentScene->CreateBoxActor(m_physics, objName, shapeName, physicstype, transform, halfExtend, density);
		return true;
	}
	else
	{
		return false;
	}
}


bool PhysicsEngine::PhsXPhysicsEngine::CreateSphereActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, float radius, float density)
{
	if (m_currentScene)
	{
		m_currentScene->CreateSphereActor(m_physics, objName, shapeName, physicstype, transform, radius, density);
		return true;
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::CreatePlainActor(const std::string& objName, const std::string& shapeName)
{
	if (m_currentScene)
	{
		m_currentScene->CreatePlainActor(m_physics, objName, shapeName);
		return true;
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::CreateCapsuleActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, float radius, float halfHeight, float density)
{
	if (m_currentScene)
	{
		m_currentScene->CreateCapsuleActor(m_physics, objName, shapeName, physicstype, transform, halfHeight, radius, density);
		return true;
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::CreateTriangleMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, ePhysicsType phsicsType, Transform transform, float density)
{
	if (m_currentScene)
	{
		m_currentScene->CreateTriangleMeshActor(m_physics, m_cooking, reinterpret_cast<physx::PxVec3*>(vertices), vertexCnt, indices, indexCnt, objName, shapeName, phsicsType, transform, density);
		return true;
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::CreateConvexMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, ePhysicsType phsicsType, Transform transform, float density)
{
	if (m_currentScene)
	{
		m_currentScene->CreateConvexMeshActor(m_physics, m_cooking, reinterpret_cast<physx::PxVec3*>(vertices), vertexCnt, indices, indexCnt, objName, shapeName, phsicsType, transform, density);
		return true;
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::DeleteActor(const std::string& objName)
{
	if (m_currentScene)
	{
		return m_currentScene->DeleteActor(objName);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetTriggerShape(const std::string& objName, bool flag)
{
	if (m_currentScene)
	{
		return m_currentScene->SetTriggerShape(objName, flag);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::DisableAcotr(const std::string& objName)
{
	if (m_currentScene)
	{
		return m_currentScene->DisableAcotr(objName);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::EnableAcotr(const std::string& objName)
{
	if (m_currentScene)
	{
		return m_currentScene->EnableAcotr(objName);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetBoxCollider(const std::string& objName, Vector3 halfExtend, float mass)
{
	return false;
}

bool PhysicsEngine::PhsXPhysicsEngine::AddForce(const std::string& objName, Vector3 force)
{
	if (m_currentScene)
	{
		return m_currentScene->AddForce(objName, Vector3ToPxVec3(force));
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::AddImpulse(const std::string& objName, Vector3 force)
{
	if (m_currentScene)
	{
		return m_currentScene->AddImpulse(objName, Vector3ToPxVec3(force));
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::AddTorque(const std::string& objName, Vector3 force)
{
	if (m_currentScene)
	{
		return m_currentScene->AddTorque(objName, Vector3ToPxVec3(force));
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::AddTorqueImpulse(const std::string& objName, Vector3 force)
{
	if (m_currentScene)
	{
		return m_currentScene->AddTorqueImpulse(objName, Vector3ToPxVec3(force));
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetLinearDamping(const std::string& objName, float damping)
{
	if (m_currentScene)
	{
		return m_currentScene->SetLinearDamping(objName, damping);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetAngularDamping(const std::string& objName, float damping)
{
	if (m_currentScene)
	{
		return m_currentScene->SetAngularDamping(objName, damping);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetLinearVelocity(const std::string& objName, Vector3 vel)
{
	if (m_currentScene)
	{
		return m_currentScene->SetLinearVelocity(objName, Vector3ToPxVec3(vel));
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetAnguarVelocity(const std::string& objName, Vector3 vel)
{
	if (m_currentScene)
	{
		return m_currentScene->SetAnguarVelocity(objName, Vector3ToPxVec3(vel));
	}
	else
	{
		return false;
	}

}

bool PhysicsEngine::PhsXPhysicsEngine::SetMaxLinearVelocity(const std::string& objName, float vel)
{
	if (m_currentScene)
	{
		return m_currentScene->SetMaxLinearVelocity(objName, vel);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetMaxAngularVelocity(const std::string& objName, float vel)
{
	if (m_currentScene)
	{
		return m_currentScene->SetMaxAngularVelocity(objName, vel);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::GetAnguarVelocity(const std::string& objName, Vector3& output)
{
	if (m_currentScene)
	{
		physx::PxVec3 ret;
		bool result = m_currentScene->GetAngularVelocity(objName, ret);
		output.vec[0] = ret.x;
		output.vec[1] = ret.y;
		output.vec[2] = ret.z;

		return result;
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::GetLinearVelocity(const std::string& objName, Vector3& output)
{
	if (m_currentScene)
	{
		physx::PxVec3 ret;
		bool result = m_currentScene->GetLinearVelocity(objName, ret);
		output.vec[0] = ret.x;
		output.vec[1] = ret.y;
		output.vec[2] = ret.z;

		return result;
	}
	else
	{
		return false;
	}
}


bool PhysicsEngine::PhsXPhysicsEngine::SetTransform(const std::string& objName, Transform transform)
{
	if (m_currentScene)
	{
		return m_currentScene->SetTransform(objName, TransformToPxTransform(transform));
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetGravity(const std::string& objName, bool flag)
{
	if (m_currentScene)
	{
		return m_currentScene->SetGravity(objName, flag);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetKinematic(const std::string& objName, bool flag)
{
	if (m_currentScene)
	{
		return m_currentScene->SetKinematic(objName, flag);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetMass(const std::string& objName, float mass)
{
	if (m_currentScene)
	{
		return m_currentScene->SetMass(objName, mass);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::SetAxisLock(const std::string& objName, PhysicsAixsLock lockInfo)
{
	if (m_currentScene)
	{
		return m_currentScene->SetAxisLock(objName, lockInfo);
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::Raycast(Vector3 origin, Vector3 dir, float distance, RayCastHit& hit)
{
	if (m_currentScene->Raycast(Vector3ToPxVec3(origin), Vector3ToPxVec3(dir), distance, hit))
	{
		return true;
	}

	return false;
}

bool PhysicsEngine::PhsXPhysicsEngine::Raycast(Vector3 origin, Vector3 dir, float distance, std::vector<std::string> Filteredlayers, ePhysicsType PhysicsFlags, RayCastHit& hit)
{
	if (m_currentScene->Raycast(Vector3ToPxVec3(origin), Vector3ToPxVec3(dir), distance, Filteredlayers, PhysicsFlags, hit))
	{
		return true;
	}

	return false;
}

bool PhysicsEngine::PhsXPhysicsEngine::AddLayer(const std::string& layerName)
{
	if (m_currentScene->AddLayer(layerName))
	{
		return true;
	}
	return false;
}

bool PhysicsEngine::PhsXPhysicsEngine::SetLayerFilterData(const std::string& layerName, std::vector<std::string> Filteredlayers)
{
	if (m_currentScene->SetLayerFilterData(layerName, Filteredlayers))
	{
		return true;
	}
	return false;
}

bool PhysicsEngine::PhsXPhysicsEngine::SetLayer(const std::string& objName, const std::string& layerName)
{
	if (m_currentScene->SetLayer(objName, layerName))
	{
		return true;
	}
	return false;
}

bool PhysicsEngine::PhsXPhysicsEngine::AddColliderEvent(std::string eventHolder, IEventCollider* callbackClass)
{
	if (m_currentScene->AddColliderEvent(eventHolder, callbackClass))
	{
		return true;
	}

	return false;
}

bool PhysicsEngine::PhsXPhysicsEngine::AddTriggerEvent(std::string eventHolder, IEventTrigger* callbackClass)
{
	if (m_currentScene->AddTriggerEvent(eventHolder, callbackClass))
	{
		return true;
	}

	return false;
}


bool PhysicsEngine::PhsXPhysicsEngine::Simualte(float deltaTime)
{
	if (m_currentScene)
	{
		m_currentScene->Simulate(deltaTime);
		return true;
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::PhsXPhysicsEngine::GetTransform(const std::string& objName, Transform& outputTrnasform)
{
	physx::PxTransform tempTransform;
	if (m_currentScene->GetTransform(objName, tempTransform))
	{
		outputTrnasform = PxTransformToTransform(tempTransform);
		return true;
	}

	return false;
}

bool PhysicsEngine::PhsXPhysicsEngine::Test()
{
	m_currentScene->Test(m_physics);

	return true;
}

void PhysicsEngine::PhsXPhysicsEngine::onRelease(const physx::PxBase* observed, void* userData, physx::PxDeletionEventFlag::Enum deletionEvent)
{
	PX_UNUSED(userData);
	PX_UNUSED(deletionEvent);

	//if (observed->is<PxRigidActor>())
	//{
	//	const PxRigidActor* actor = static_cast<const PxRigidActor*>(observed);

	//	removeRenderActorsFromPhysicsActor(actor);

	//	std::vector<PxRigidActor*>::iterator actorIter = std::find(mPhysicsActors.begin(), mPhysicsActors.end(), actor);
	//	if (actorIter != mPhysicsActors.end())
	//	{
	//		mPhysicsActors.erase(actorIter);
	//	}
	//}
}

physx::PxDefaultErrorCallback PhysicsEngine::PhsXPhysicsEngine::m_defaultErrorCallback;
physx::PxDefaultAllocator PhysicsEngine::PhsXPhysicsEngine::m_defaultAllocatorCallback;

