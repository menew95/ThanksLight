#pragma once
#include "IPhysicsEngine.h"
#define PVD_HOST "127.0.0.1"

/// <summary>
/// PhysX ����
/// 
/// ToDO::::: 
///	���������� �������� �����ϴ� �������δٰ� ������ ��. �������ϱ� ���߿�.....
/// 
/// </summary>

namespace PhysicsEngine
{
	class Scene;

	class PhsXPhysicsEngine : public IPhysicsEngine, public physx::PxDeletionListener
	{
	public:
		PhsXPhysicsEngine();
		~PhsXPhysicsEngine() override;

		bool Initialize() override;
		bool Finalalize() override;

	public:
		//////////////////// IPhysics Implement ////////////////

		//Scene ���� �Լ���
		virtual bool AddScene(const std::string& sceneName) override;
		virtual bool SetCurrentScene(const std::string& sceneName) override;
		virtual bool DeleteScene(const std::string& sceneName) override;

		//���� Scene�� ��ü ����
		virtual bool CreateBoxActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, Vector3 halfExtend, float density) override;
		virtual bool CreateSphereActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, float radius, float density) override;
		virtual bool CreatePlainActor(const std::string& objName, const std::string& shapeName) override;
		virtual bool CreateCapsuleActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, float radius, float halfHeight, float density) override;
		virtual bool CreateTriangleMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, ePhysicsType phsicsType, Transform transform, float density) override;
		virtual bool CreateConvexMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, ePhysicsType phsicsType, Transform transform, float density) override;

		virtual bool DeleteActor(const std::string& objName) override;

		//���� Ʈ����ȭ
		virtual bool SetTriggerShape(const std::string& objName, bool flag) override;

		//��Ȱ��ȭ, �������� ����, ����ĳ���ÿ��� ���ܵ˴ϴ�.
		virtual bool DisableAcotr(const std::string& objName) override;
		virtual bool EnableAcotr(const std::string& objName) override;

		//���� Scene�� ��ü�� ������(Collider) ũ�� ����
		virtual bool SetBoxCollider(const std::string& objName, Vector3 halfExtend, float mass) override;

		virtual bool AddForce(const std::string& objName, Vector3 force) override;
		virtual bool AddImpulse(const std::string& objName, Vector3 force) override;
		virtual bool AddTorque(const std::string& objName, Vector3 force) override;
		virtual bool AddTorqueImpulse(const std::string& objName, Vector3 force) override;
		virtual bool SetLinearDamping(const std::string& objName, float damping) override;
		virtual bool SetAngularDamping(const std::string& objName, float damping) override;
		virtual bool SetLinearVelocity(const std::string& objName, Vector3 vel) override;
		virtual bool SetAnguarVelocity(const std::string& objName, Vector3 vel) override;

		virtual bool SetMaxLinearVelocity(const std::string& objName, float vel) override;
		virtual bool SetMaxAngularVelocity(const std::string& objName, float vel) override;

		virtual bool GetAnguarVelocity(const std::string& objName, Vector3& output) override;
		virtual bool GetLinearVelocity(const std::string& objName, Vector3& output) override;

		virtual bool SetTransform(const std::string& objName, Transform transform) override;
		virtual bool SetGravity(const std::string& objName, bool flag) override;
		virtual bool SetKinematic(const std::string& objName, bool flag) override;
		virtual bool SetMass(const std::string& objName, float mass) override;
		virtual bool SetAxisLock(const std::string& objName, PhysicsAixsLock lockInfo) override;

		//RayCast ������, ����, �Ÿ�, outHit
		virtual bool Raycast(Vector3 origin, Vector3 dir, float distance, RayCastHit& hit) override;
		//RayCast ������, ����, �Ÿ�, ���͸� �� ���̾�, �浹�� ������Ʈ Ÿ��, outHit;
		virtual bool Raycast(Vector3 origin, Vector3 dir, float distance, std::vector<std::string> Filteredlayers, ePhysicsType PhysicsFlags, RayCastHit& hit) override;

		//�浹���̾� ������ ���� ���̾� �ִ� 31������ �߰�����
		virtual bool AddLayer(const std::string& layerName) override;
		//FilteredLayer�� �ִ� �̸��� ���� ���̾�� �浹�� ���õȴ�. ����� ŭ. ���� �ʱ�ȭ�� �߰��ϰ�, ����ŷ �����Ұ�. 
		//�߰������� �ǳ�, ������ �ִ� ������Ʈ�鿡�� ������ �ȵ� �� ����. ���׹߻� Ȯ�� �ſ������
		virtual bool SetLayerFilterData(const std::string& layerName, std::vector<std::string> Filteredlayers) override;
		//������Ʈ�� ���̾ �����Ѵ�. ������Ʈ���� defaultSetting�� ��� default�̴�.
		virtual bool SetLayer(const std::string& objName, const std::string& layerName) override;

		//�̺�Ʈ
		virtual bool AddColliderEvent(std::string eventHolder, IEventCollider*) override;
		virtual bool AddTriggerEvent(std::string eventHolder, IEventTrigger*)  override;

		virtual bool Simualte(float deltaTime) override;
		virtual bool GetTransform(const std::string& objName, Transform& outputTrnasform) override;



		virtual bool Test() override;

		//////////////////// Functions ////////////////

	private:
		virtual void onRelease(const physx::PxBase* observed, void* userData, physx::PxDeletionEventFlag::Enum deletionEvent);

	private:

		static physx::PxDefaultErrorCallback m_defaultErrorCallback;
		static physx::PxDefaultAllocator m_defaultAllocatorCallback;

		physx::PxFoundation* m_foundation;
		physx::PxPhysics* m_physics;
		physx::PxTolerancesScale m_toleranceScale;

		physx::PxDefaultCpuDispatcher* m_cpuDispatcher;
		physx::PxCooking* m_cooking;

		physx::PxPvd* m_pvd;
		physx::PxPvdTransport* m_pvdTransport;

		//��������
		std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_currentScene;
	};
}

