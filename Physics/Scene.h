#pragma once

/// <summary>
/// ���������� ���� ����(Scene)
/// </summary>

namespace PhysicsEngine
{
	class EventCallback;

	class Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		Scene() = default;
		~Scene() = default;

		bool Initialize(physx::PxPhysics* physics, physx::PxCpuDispatcher* dispatcher, physx::PxTolerancesScale& scale);
		bool Finalize();

	public:
		using SCENE_LOCK = physx::PxSceneWriteLock;

		bool Simulate(float dt);

		//����
		bool CreateBoxActor     (physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName, ePhysicsType physicsType ,Transform transform, Vector3 halfExtend, float density);
		bool CreatePlainActor   (physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName);
		bool CreateSphereActor  (physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName, ePhysicsType phsicsType, Transform transform, float rad, float density);
		//to do : �ڵ� �ۼ��� ��.
		bool CreateCapsuleActor	(physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName, ePhysicsType phsicsType, Transform transform, float halfHeight, float rad, float density);
		bool CreateConvexMeshActor(physx::PxPhysics* physics, physx::PxCooking* cooking, physx::PxVec3* vertices, float vertexCnt, physx::PxU32* indices, float indexCnt, const std::string& objName, const std::string& shapeName, ePhysicsType phsicsType, Transform transform, float density);
		bool CreateTriangleMeshActor (physx::PxPhysics* physics, physx::PxCooking* cooking, physx::PxVec3* vertices, float vertexCnt, physx::PxU32* indices, float indexCnt, const std::string& objName, const std::string& shapeName, ePhysicsType phsicsType, Transform transform, float density);

		//Ʈ���� ����
		bool SetTriggerShape(const std::string& objName, bool flag);

		//������ ����
		bool ArrangeBoxActor    (physx::PxPhysics* physics, const std::string& objName, Vector3 halfExtend, float density);
		bool ArrangePlainActor  (physx::PxPhysics* physics, const std::string& objName);
		bool ArrangeSphereActor (physx::PxPhysics* physics, const std::string& objName, float rad, float density);
		//to do : �ڵ� �ۼ��� ��.
		bool ArrangeCapsuleActor(physx::PxPhysics* physics, const std::string& objName);
		//bool ArrangeMeshShape   (physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName);

		//����
		bool DeleteActor(const std::string& objName);
		//��Ȱ��ȭ, �������� �������� ���ܵ˴ϴ�.
		bool DisableAcotr(const std::string& objName);
		bool EnableAcotr(const std::string& objName);

		//Actor ����
		// ��������
		bool ClearAllForce(const std::string& objName);

		bool AddForce(const std::string& objName, physx::PxVec3 force);
		bool AddImpulse(const std::string& objName, physx::PxVec3 force);
		bool AddTorque(const std::string& objName, physx::PxVec3 force);
		bool AddTorqueImpulse(const std::string& objName, physx::PxVec3 force);

		//���� , drag(�׷�) ������Ƽ
		bool SetLinearDamping(const std::string& objName, float damping);
		bool SetAngularDamping(const std::string& objName, float damping);

		//
		bool SetLinearVelocity(const std::string& objName, physx::PxVec3 vel);
		bool SetAnguarVelocity(const std::string& objName, physx::PxVec3 vel);
		
		bool SetMaxLinearVelocity(const std::string& objName, physx::PxReal vel);
		bool SetMaxAngularVelocity(const std::string& objName, physx::PxReal vel);

		bool SetKinematic(const std::string& objName, bool flag);
		bool SetGravity(const std::string& objName, bool flag);
		bool SetAxisLock(const std::string& objName, PhysicsAixsLock lockInfo);
		bool SetTransform(const std::string& objName, physx::PxTransform transform);
		bool SetMass(const std::string& objName, float mass);

		bool GetLinearVelocity(const std::string& objName, physx::PxVec3& output);
		bool GetAngularVelocity(const std::string& objName, physx::PxVec3& output);

		bool Raycast(physx::PxVec3 origin, physx::PxVec3 dir, float distance, std::vector<std::string>& Filteredlayers, ePhysicsType PhysicsFlags, RayCastHit& hit);
		bool Raycast(physx::PxVec3 origin, physx::PxVec3 dir, float distance, RayCastHit& hit);

		bool GetTransform(const std::string& objName, physx::PxTransform& transform);

		//�̺�Ʈ ����
		bool AddColliderEvent(std::string eventHolder, IEventCollider* callbackclass);
		bool AddTriggerEvent(std::string eventHolder, IEventTrigger* callbackclass);

		//�浹���̾� ������ ���� ���̾� �ִ� 31������ �߰�����
		bool AddLayer(const std::string& layerName); 
		//FilteredLayer�� �ִ� �̸��� ���� ���̾�� �浹�� ���õȴ�. ����� ŭ. ���� �ʱ�ȭ�� �߰��ϰ�, ����ŷ �����Ұ�. 
		//�߰������� �ǳ�, ������ �ִ� ������Ʈ�鿡�� ������ �ȵ� �� ����. ���׹߻� Ȯ�� �ſ������
		bool SetLayerFilterData(const std::string& layerName, std::vector<std::string> Filteredlayers);
		//������Ʈ�� ���̾ �����Ѵ�. ������Ʈ���� defaultSetting�� ��� default�̴�.
		bool SetLayer(const std::string& objName, const std::string& layerName);

		void Test(physx::PxPhysics* physics);



	private:
		physx::PxScene* m_scene;

		//������Ʈ �̸�
		std::unordered_map<std::string, physx::PxRigidActor*> m_rigidActors;

		std::unordered_map<std::string, physx::PxRigidActor*> m_ContactEvents;
		std::unordered_map<std::string, physx::PxRigidActor*> m_TriggerEvents;

		//�ڱ��ڽ��� ���̾��̸�, <����ID, ���͸���ũ>
		std::unordered_map<std::string, std::pair<physx::PxU32, physx::PxU32>> m_Layer;

		physx::PxMaterial* m_material;
		std::shared_ptr<EventCallback> m_eventCallback;

		//������
		//std::unordered_map<std::string, physx::PxShape*> m_Shapes;
	};

	physx::PxFilterFlags PhysicsWorldFilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize
	);
}

