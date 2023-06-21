#include "pch.h"
#include "Scene.h"
#include "PxSceneLock.h"
#include "EventCallback.h"

bool PhysicsEngine::Scene::Initialize(physx::PxPhysics* physics, physx::PxCpuDispatcher* dispatcher, physx::PxTolerancesScale& scale)
{
	physx::PxSceneDesc sceneDesc(scale);

	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81 * 1.5f, 0.0f);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PhysicsEngine::PhysicsWorldFilterShader;
	//sceneDesc.simulationEventCallback = ;
	//gjk algorithm 콜리전 디텍션 시스템 
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
	////액티브 액터 체킹
	//sceneDesc.sceneQueryUpdateMode = physx::PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;

	m_scene = physics->createScene(sceneDesc);
	//m_scene->SetFlag(physx::PxSceneFlag::, true);

#if defined(DEBUG) || defined(_DEBUG)
	//m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
	m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

	physx::PxPvdSceneClient* pvdClient = m_scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
#endif

	m_material = physics->createMaterial(0.7f, 0.7f, 0.05f);

	m_eventCallback = std::make_shared<EventCallback>();
	m_scene->setSimulationEventCallback(m_eventCallback.get());

	physx::PxU32 defaultLayerFilterID = 0x00000001;
	physx::PxU32 defaultLayerFilterMask = 0xFFFFFFFF;

	m_Layer.emplace("Default", std::make_pair(defaultLayerFilterID, defaultLayerFilterMask));

	return true;
}

bool PhysicsEngine::Scene::Finalize()
{

	for (auto actors : m_rigidActors)
	{
		if (actors.second->userData != nullptr)
		{
			auto data = reinterpret_cast<UserData*>(actors.second->userData);
			delete data;
			actors.second->userData = nullptr;
		}
	}
	m_scene->release();
	m_rigidActors.clear();


	return true;
}

bool PhysicsEngine::Scene::Simulate(float dt)
{
	SCENE_LOCK sceneLock(*m_scene);

	if (dt > 0.0f)
	{
		for (auto actors : m_rigidActors)
		{
			if (actors.second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
			{
				((physx::PxRigidDynamic*)actors.second)->setMaxDepenetrationVelocity(3.0f);
			}
		}

		m_scene->simulate(dt);
		m_scene->fetchResults(true);
		m_eventCallback->CallonTriggerPersist();
	}

	return true;
}

bool PhysicsEngine::Scene::CreateBoxActor(physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName, ePhysicsType phsicsType, Transform transform, Vector3 halfExtend, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor != m_rigidActors.end())
	{
		return false;
	}

	physx::PxTransform pxTransform = TransformToPxTransform(transform);
	physx::PxShape* shape = physics->createShape(physx::PxBoxGeometry(halfExtend.vec[0], halfExtend.vec[1], halfExtend.vec[2]), *m_material, true);
	physx::PxRigidActor* box = nullptr;

	physx::PxFilterData filterData;
	filterData.word0 = 0x00000001;//자기자신
	filterData.word1 = 0xFFFFFFFF;//다른 상대 오브젝트들
	shape->setSimulationFilterData(filterData);


	switch (phsicsType)
	{
	case ePhysicsType::eSTATTIC:
	{
		box = physx::PxCreateStatic(*physics, TransformToPxTransform(transform), *shape);
		break;
	}
	case ePhysicsType::eDYNAMIC:
	{
		box = physx::PxCreateDynamic(*physics, TransformToPxTransform(transform), *shape, density);
		break;
	}
	default:
	{
		return false;
	}
	}

	m_scene->addActor(*box);

	UserData* data = new UserData;
	data->m_name = objName;
	box->userData = data;
	m_rigidActors.emplace(objName, (box));

	shape->release();

	return true;
}

bool PhysicsEngine::Scene::CreatePlainActor(physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor != m_rigidActors.end())
	{
		return false;
	}

	auto foundedPlain = m_rigidActors.find("Plain");
	physx::PxMaterial* material = nullptr;
	if (foundedPlain == m_rigidActors.end())
	{
		material = physics->createMaterial(0.5f, 0.5f, 0.6f);
	}
	else
	{
		//material = foundedPlain->second;
	}

	physx::PxRigidActor* plain = physx::PxCreatePlane(*physics, physx::PxPlane(0, 1, 0, 15), *material);

	UserData* data = new UserData;
	data->m_name = objName;
	plain->userData = data;

	m_rigidActors.emplace(objName, plain);
	m_scene->addActor(*plain);

	return true;
}


bool PhysicsEngine::Scene::CreateSphereActor(physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName, ePhysicsType phsicsType, Transform transform, float rad, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor != m_rigidActors.end())
	{
		return false;
	}

	physx::PxFilterData filterData;
	filterData.word0 = 0x00000001;//자기자신
	filterData.word1 = 0xFFFFFFFF;//다른 상대 오브젝트들

	physx::PxTransform pxTransform = TransformToPxTransform(transform);
	physx::PxShape* shape = physics->createShape(physx::PxSphereGeometry(rad), *m_material, true);
	shape->setSimulationFilterData(filterData);
	filterData.word1 = 0x00000000;
	shape->setQueryFilterData(filterData);

	//physx::PxRigidDynamic* box = physics->createRigidDynamic(pxTransform);
	physx::PxRigidActor* sphere = nullptr;

	switch (phsicsType)
	{
	case ePhysicsType::eSTATTIC:
	{
		sphere = physx::PxCreateStatic(*physics, TransformToPxTransform(transform), *shape);
		break;
	}
	case ePhysicsType::eDYNAMIC:
	{
		sphere = physx::PxCreateDynamic(*physics, TransformToPxTransform(transform), *shape, density);
		break;
	}
	default:
	{
		return false;
	}
	}

	UserData* data = new UserData;
	data->m_name = objName;
	sphere->userData = data;

	m_scene->addActor(*sphere);
	m_rigidActors.emplace(objName, (sphere));

	shape->release();

	return true;
}

bool PhysicsEngine::Scene::CreateCapsuleActor(physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName, ePhysicsType phsicsType, Transform transform, float halfHeight, float rad, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor != m_rigidActors.end())
	{
		return false;
	}

	physx::PxFilterData filterData;
	filterData.word0 = 0x00000001;//자기자신
	filterData.word1 = 0xFFFFFFFF;//다른 상대 오브젝트들

	physx::PxTransform pxTransform = TransformToPxTransform(transform);
	physx::PxShape* shape = physics->createShape(physx::PxCapsuleGeometry(rad, halfHeight), *m_material, true);
	shape->setSimulationFilterData(filterData);
	filterData.word1 = 0x00000000;
	shape->setQueryFilterData(filterData);

	physx::PxRigidActor* capsule = nullptr;

	switch (phsicsType)
	{
	case ePhysicsType::eSTATTIC:
	{
		capsule = physx::PxCreateStatic(*physics, TransformToPxTransform(transform), *shape);
		break;
	}
	case ePhysicsType::eDYNAMIC:
	{
		capsule = physx::PxCreateDynamic(*physics, TransformToPxTransform(transform), *shape, density);
		break;
	}
	default:
	{
		return false;
	}
	}

	UserData* data = new UserData;
	data->m_name = objName;
	capsule->userData = data;

	m_scene->addActor(*capsule);
	m_rigidActors.emplace(objName, (capsule));

	shape->release();
	return false;

}

bool PhysicsEngine::Scene::CreateConvexMeshActor(physx::PxPhysics* physics, physx::PxCooking* cooking, physx::PxVec3* vertices, float vertexCnt, physx::PxU32* indices, float indexCnt, const std::string& objName, const std::string& shapeName, ePhysicsType phsicsType, Transform transform, float density)
{
	SCENE_LOCK sceneLock(*m_scene);


	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor != m_rigidActors.end())
	{
		return false;
	}

	physx::PxConvexMeshDesc  meshDesc;
	meshDesc.points.count = vertexCnt;
	meshDesc.points.stride = sizeof(physx::PxVec3);
	meshDesc.points.data = vertices;

	meshDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
	//meshDesc.polygons.count = indexCnt;
	//meshDesc.polygons.stride = 3 * sizeof(physx::PxHullPolygon);
	//meshDesc.polygons.data = indices;

	physx::PxDefaultMemoryOutputStream writeBuffer;

	physx::PxConvexMeshCookingResult::Enum result;

	bool status = cooking->cookConvexMesh(meshDesc, writeBuffer, &result);
	if (!status)
		return NULL;

	physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	physx::PxConvexMesh* convexMesh = physics->createConvexMesh(readBuffer);

	physx::PxShape* aConvexShape = physics->createShape(physx::PxConvexMeshGeometry(convexMesh, physx::PxMeshScale({0.25f, 0.25f, 0.25f})), *m_material, true);

	physx::PxFilterData filterData;
	filterData.word0 = 0x00000001;//자기자신
	filterData.word1 = 0xFFFFFFFF;//다른 상대 오브젝트들

	physx::PxTransform pxTransform = TransformToPxTransform(transform);

	aConvexShape->setSimulationFilterData(filterData);
	filterData.word1 = 0x00000000;
	aConvexShape->setQueryFilterData(filterData);

	physx::PxRigidActor* actor = nullptr;

	switch (phsicsType)
	{
	case ePhysicsType::eSTATTIC:
	{
		actor = physx::PxCreateStatic(*physics, TransformToPxTransform(transform), *aConvexShape);
		break;
	}
	case ePhysicsType::eDYNAMIC:
	{
		actor = physx::PxCreateDynamic(*physics, TransformToPxTransform(transform), *aConvexShape, density);
		break;
	}
	default:
	{
		return false;
	}
	}

	UserData* data = new UserData;
	data->m_name = objName;
	actor->userData = data;

	m_scene->addActor(*actor);
	m_rigidActors.emplace(objName, (actor));

	aConvexShape->release();
}

bool PhysicsEngine::Scene::CreateTriangleMeshActor(physx::PxPhysics* physics, physx::PxCooking* cooking, physx::PxVec3* vertices, float vertexCnt, physx::PxU32* indices, float indexCnt, const std::string& objName, const std::string& shapeName, ePhysicsType phsicsType, Transform transform, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	physx::PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = vertexCnt;
	meshDesc.points.stride = sizeof(physx::PxVec3);
	meshDesc.points.data = vertices;

	meshDesc.triangles.count = indexCnt / 3;
	meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
	meshDesc.triangles.data = indices;

	physx::PxDefaultMemoryOutputStream writeBuffer;
	physx::PxTriangleMeshCookingResult::Enum result;
	bool status = cooking->cookTriangleMesh(meshDesc, writeBuffer, &result);
	if (!status)
		return NULL;

	physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	auto TriangleMesh = physics->createTriangleMesh(readBuffer);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor != m_rigidActors.end())
	{
		return false;
	}

	physx::PxFilterData filterData;
	filterData.word0 = 0x00000001;//자기자신
	filterData.word1 = 0xFFFFFFFF;//다른 상대 오브젝트들

	physx::PxTransform pxTransform = TransformToPxTransform(transform);

	physx::PxShape* shape = physics->createShape(physx::PxTriangleMeshGeometry(TriangleMesh), *m_material, true);

	shape->setSimulationFilterData(filterData);
	filterData.word1 = 0x00000000;
	shape->setQueryFilterData(filterData);

	physx::PxRigidActor* actor = nullptr;

	switch (phsicsType)
	{
	case ePhysicsType::eSTATTIC:
	{
		actor = physx::PxCreateStatic(*physics, TransformToPxTransform(transform), *shape);
		break;
	}
	case ePhysicsType::eDYNAMIC:
	{
		actor = physx::PxCreateKinematic(*physics, TransformToPxTransform(transform), *shape, density);
		break;
	}
	default:
	{
		return false;
	}
	}

	UserData* data = new UserData;
	data->m_name = objName;
	actor->userData = data;

	m_scene->addActor(*actor);
	m_rigidActors.emplace(objName, (actor));

	shape->release();
}

bool PhysicsEngine::Scene::SetTriggerShape(const std::string& objName, bool flag)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		physx::PxShape** oldShape = new physx::PxShape*;

		foundedActor->second->getShapes(oldShape, 1);

		if (oldShape != nullptr)
		{
			(*oldShape)->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !flag);
			(*oldShape)->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, flag);

			delete oldShape;
			return true;
		}
		delete oldShape;
	}

	return false;
}


bool PhysicsEngine::Scene::ArrangeBoxActor(physx::PxPhysics* physics, const std::string& objName, Vector3 halfExtend, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		physx::PxShape** oldShape = nullptr;

		foundedActor->second->getShapes(oldShape, 1);
		(*oldShape)->release();

		physx::PxShape* shape = physics->createShape(physx::PxBoxGeometry(halfExtend.vec[0], halfExtend.vec[1], halfExtend.vec[2]), *m_material);

		foundedActor->second->attachShape(*shape);
		return true;
	}

	return false;

}

bool PhysicsEngine::Scene::ArrangePlainActor(physx::PxPhysics* physics, const std::string& objName)
{
	SCENE_LOCK sceneLock(*m_scene);
	//physx::PxRigidActor* plain = physx::PxCreatePlane(*physics, physx::PxPlane(0, 1, 0, 0), *material);
	return false;
}

bool PhysicsEngine::Scene::ArrangeSphereActor(physx::PxPhysics* physics, const std::string& objName, float rad, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		physx::PxShape** oldShape = nullptr;

		foundedActor->second->getShapes(oldShape, 1);
		(*oldShape)->release();

		physx::PxShape* shape = physics->createShape(physx::PxSphereGeometry(rad), *m_material);

		foundedActor->second->attachShape(*shape);
		return true;
	}

	return false;
}


bool PhysicsEngine::Scene::ClearAllForce(const std::string& objName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->setLinearVelocity({ 0.f, 0.f, 0.f });
			((physx::PxRigidDynamic*)foundedActor->second)->setAngularVelocity({ 0.f, 0.f, 0.f });

			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::AddForce(const std::string& objName, physx::PxVec3 force)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->addForce(force, physx::PxForceMode::eFORCE, true);

			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::AddImpulse(const std::string& objName, physx::PxVec3 force)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->addForce(force, physx::PxForceMode::eIMPULSE, true);

			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::AddTorque(const std::string& objName, physx::PxVec3 force)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->addTorque(force, physx::PxForceMode::eFORCE, true);

			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::AddTorqueImpulse(const std::string& objName, physx::PxVec3 force)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->addTorque(force, physx::PxForceMode::eIMPULSE, true);

			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::SetLinearDamping(const std::string& objName, float damping)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->setLinearDamping(damping);

			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::SetAngularDamping(const std::string& objName, float damping)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->setAngularDamping(damping);

			return true;
		}
		return false;
	}
	return false;
}

bool PhysicsEngine::Scene::SetLinearVelocity(const std::string& objName, physx::PxVec3 vel)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->setLinearVelocity(vel);

			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::SetAnguarVelocity(const std::string& objName, physx::PxVec3 vel)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->setAngularVelocity(vel);

			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::SetMaxLinearVelocity(const std::string& objName, physx::PxReal vel)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->setMaxLinearVelocity(vel);

			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::SetMaxAngularVelocity(const std::string& objName, physx::PxReal vel)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->setMaxAngularVelocity(vel);

			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::SetAxisLock(const std::string& objName, PhysicsAixsLock lockInfo)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{

			((physx::PxRigidDynamic*)foundedActor->second)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, lockInfo.m_linearX);
			((physx::PxRigidDynamic*)foundedActor->second)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, lockInfo.m_linearY);
			((physx::PxRigidDynamic*)foundedActor->second)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, lockInfo.m_linearZ);
			((physx::PxRigidDynamic*)foundedActor->second)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, lockInfo.m_anguarX);
			((physx::PxRigidDynamic*)foundedActor->second)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, lockInfo.m_anguarY);
			((physx::PxRigidDynamic*)foundedActor->second)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, lockInfo.m_anguarZ);

			return true;
		}
	}
	return false;

}

bool PhysicsEngine::Scene::SetTransform(const std::string& objName, physx::PxTransform transform)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		foundedActor->second->setGlobalPose(transform);
		return true;
	}
	return false;
}

bool PhysicsEngine::Scene::SetKinematic(const std::string& objName, bool flag)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, flag);
			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::SetGravity(const std::string& objName, bool flag)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (flag)
		{
			foundedActor->second->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
		}
		else
		{
			foundedActor->second->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
		}
	}
	return false;
}

bool PhysicsEngine::Scene::SetMass(const std::string& objName, float mass)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			bool result = physx::PxRigidBodyExt::setMassAndUpdateInertia(*((physx::PxRigidDynamic*)foundedActor->second), mass);
			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::GetLinearVelocity(const std::string& objName, physx::PxVec3& output)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			output = ((physx::PxRigidDynamic*)foundedActor->second)->getLinearVelocity();
			return true;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::GetAngularVelocity(const std::string& objName, physx::PxVec3& output)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			output = ((physx::PxRigidDynamic*)foundedActor->second)->getAngularVelocity();
			return true;
		}
	}
	return false;
}


bool PhysicsEngine::Scene::Raycast(physx::PxVec3 origin, physx::PxVec3 dir, float distance, std::vector<std::string>& Filteredlayers, ePhysicsType PhysicsFlags, RayCastHit& hit)
{
	SCENE_LOCK sceneLock(*m_scene);

	const physx::PxHitFlags outputFlags = physx::PxHitFlag::ePOSITION | physx::PxHitFlag::eNORMAL;


	physx::PxQueryFilterData filterData;

	filterData.data.word0 = 0xFFFFFFFF;

	for (auto filterLayer : Filteredlayers)
	{
		auto foundLayer = m_Layer.find(filterLayer);
		if (foundLayer == m_Layer.end())
		{
			continue;
		}

		filterData.data.word0 -= foundLayer->second.first;
	}

	switch (PhysicsFlags)
	{
	case PhysicsEngine::ePhysicsType::eSTATTIC:
	{
		filterData.flags = physx::PxQueryFlag::eSTATIC;
		break;
	}
	case PhysicsEngine::ePhysicsType::eDYNAMIC:
	{
		filterData.flags = physx::PxQueryFlag::eDYNAMIC;
		break;
	}
	case PhysicsEngine::ePhysicsType::ePhysicsType_COUNT:
	case PhysicsEngine::ePhysicsType::INVAILD:
	default:
	{
		break;
	}
	}

	physx::PxRaycastBuffer buf1;

	bool result = m_scene->raycast(origin, dir, distance, buf1, physx::PxHitFlags(physx::PxHitFlag::eDEFAULT), filterData);

	if (buf1.hasBlock)
	{
		if (buf1.block.actor->userData == nullptr)
		{
			return false;
		}
		auto type = buf1.block.actor->getType();


		hit.m_objectName = ((UserData*)buf1.block.actor->userData)->m_name;
		hit.m_distance = buf1.block.distance;

		return true;

	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::Scene::Raycast(physx::PxVec3 origin, physx::PxVec3 dir, float distance, RayCastHit& hit)
{
	SCENE_LOCK sceneLock(*m_scene);

	const physx::PxHitFlags outputFlags = physx::PxHitFlag::ePOSITION | physx::PxHitFlag::eNORMAL;

	physx::PxQueryFilterData filterData;
	filterData.data.word0 = 1 << 2 | 1 << 3 | 1;
	filterData.flags = physx::PxQueryFlag::eDYNAMIC;

	physx::PxRaycastBuffer buf1;

	bool result = m_scene->raycast(origin, dir, distance, buf1, physx::PxHitFlags(physx::PxHitFlag::eDEFAULT), filterData);

	if (buf1.hasBlock)
	{

		if (buf1.block.actor->userData == nullptr)
		{
			return false;
		}
		auto type = buf1.block.actor->getType();

		hit.m_objectName = ((UserData*)buf1.block.actor->userData)->m_name;
		hit.m_distance = buf1.block.distance;

		return true;
	}
	else
	{
		return false;
	}
}

bool PhysicsEngine::Scene::DeleteActor(const std::string& objName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		m_scene->removeActor(*(foundedActor->second));
		auto data = reinterpret_cast<UserData*>(foundedActor->second->userData);
		delete data;

		foundedActor->second->userData = nullptr;
		foundedActor->second->release();
		m_rigidActors.erase(objName);

		m_eventCallback->DeleteEvents(objName);
	}
	return false;
}

bool PhysicsEngine::Scene::DisableAcotr(const std::string& objName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		auto rigidActor = foundedActor->second;

		UINT numberOfShape = rigidActor->getNbShapes();
		std::vector<physx::PxShape*> shapes;
		shapes.resize(numberOfShape);

		rigidActor->getShapes(&shapes.front(), numberOfShape);
		for (auto shape : shapes)
		{
			shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, false);
			//shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		}
		rigidActor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);

		return true;
	}

	return false;
}

bool PhysicsEngine::Scene::EnableAcotr(const std::string& objName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		auto rigidActor = foundedActor->second;

		UINT numberOfShape = rigidActor->getNbShapes();
		std::vector<physx::PxShape*> shapes;
		shapes.resize(numberOfShape);

		rigidActor->getShapes(&shapes.front(), numberOfShape);
		for (auto shape : shapes)
		{
			shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
			//shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		}
		rigidActor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);

		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			((physx::PxRigidDynamic*)foundedActor->second)->wakeUp();
		}
	}

	return false;
}


bool PhysicsEngine::Scene::GetTransform(const std::string& objName, physx::PxTransform& transform)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		if (foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			//현재 상호작용을 하고있는(물리적힘을 받고있는) 녀석만 업데이트해줌.
			if (!((physx::PxRigidDynamic*)foundedActor->second)->isSleeping())
			{
				transform = ((physx::PxRigidDynamic*)foundedActor->second)->getGlobalPose();
				return true;
			}
			return false;
		}
	}
	return false;
}

bool PhysicsEngine::Scene::AddColliderEvent(std::string eventHolder, IEventCollider* callbackclass)
{
	return m_eventCallback->AddColliderEvent(eventHolder, callbackclass);
}

bool PhysicsEngine::Scene::AddTriggerEvent(std::string eventHolder, IEventTrigger* callbackclass)
{
	return m_eventCallback->AddTriggerEvent(eventHolder, callbackclass);
}

bool PhysicsEngine::Scene::AddLayer(const std::string& layerName)
{
	UINT layerCnt = m_Layer.size();

	if (layerCnt > 31)
	{
		return false;
	}

	physx::PxU32 filterID = 1 << layerCnt;
	physx::PxU32 DefaultfilterMask = 0xFFFFFFFF;

	m_Layer.emplace(layerName, std::make_pair(filterID, DefaultfilterMask));

	return false;
}

bool PhysicsEngine::Scene::SetLayerFilterData(const std::string& layerName, std::vector<std::string> Filteredlayers)
{
	physx::PxU32 filterMask = 0xFFFFFFFF;

	auto currentSettingLayer = m_Layer.find(layerName);
	if (currentSettingLayer == m_Layer.end())
	{
		return false;
	}

	for (auto filterLayer : Filteredlayers)
	{
		auto foundLayer = m_Layer.find(filterLayer);
		if (foundLayer == m_Layer.end())
		{
			continue;
		}

		filterMask -= foundLayer->second.first;
	}

	currentSettingLayer->second.second = filterMask;
	return false;
}


bool PhysicsEngine::Scene::SetLayer(const std::string& objName, const std::string& layerName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor == m_rigidActors.end())
	{
		return false;
	}
	else
	{
		auto foundLayer = m_Layer.find(layerName);
		if (foundLayer == m_Layer.end())
		{
			return false;
		}

		physx::PxFilterData filterData;

		filterData.word0 = foundLayer->second.first;
		filterData.word1 = foundLayer->second.second;

		physx::PxShape** shapes = new physx::PxShape*;
		(*shapes) = nullptr;
		foundedActor->second->getShapes(shapes, 1);
		if (*shapes != nullptr)
		{
			(*shapes)->setSimulationFilterData(filterData);
			filterData.word1 = 0x00000000;
			(*shapes)->setQueryFilterData(filterData);
		}

		delete shapes;
	}
	return false;
}

void PhysicsEngine::Scene::Test(physx::PxPhysics* physics)
{
	SCENE_LOCK sceneLock(*m_scene);

	physx::PxMaterial* mMaterial = NULL;

	mMaterial = physics->createMaterial(0.6f, 0.6f, 0.2f);
	physx::PxRigidStatic* groundPlane = PxCreatePlane(*physics, physx::PxPlane(0, 1.f, 0, 10.f), *mMaterial);
	physx::PxShape** shapes = new physx::PxShape*;
	physx::PxFilterData filterData;

	filterData.word0 = 0x00000001;
	filterData.word1 = 0xFFFFFFFF;

	(*shapes) = nullptr;
	groundPlane->getShapes(shapes, 1);
	if (*shapes != nullptr)
	{
		(*shapes)->setSimulationFilterData(filterData);
	}

	delete shapes;
	m_scene->addActor(*groundPlane);

}


physx::PxFilterFlags PhysicsEngine::PhysicsWorldFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	PX_UNUSED(constantBlock);
	PX_UNUSED(constantBlockSize);

	// let triggers through
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlags();
	}

	// Checking if layers should be ignored
	auto const layerMaskA = filterData0.word0;
	auto const layerA = filterData0.word1;

	auto const layerMaskB = filterData1.word0;
	auto const layerB = filterData1.word1;

	auto const aCollision = layerMaskA & layerB;
	auto const bCollision = layerMaskB & layerA;
	if (aCollision == 0 || bCollision == 0)
	{
		return physx::PxFilterFlag::eSUPPRESS;
	}

	// all initial and persisting reports for everything, with per-point data
	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT | physx::PxPairFlag::eCONTACT_DEFAULT;

	return physx::PxFilterFlag::eDEFAULT;
}