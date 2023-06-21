#pragma once
#include "IPhysicsEngine.h"
#define PVD_HOST "127.0.0.1"

/// <summary>
/// PhysX 래핑
/// 
/// ToDO::::: 
///	공유가능한 쉐이프를 공유하는 형식으로다가 만들어야 함. 귀찮으니까 나중에.....
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

		//Scene 관련 함수들
		virtual bool AddScene(const std::string& sceneName) override;
		virtual bool SetCurrentScene(const std::string& sceneName) override;
		virtual bool DeleteScene(const std::string& sceneName) override;

		//현재 Scene에 객체 생성
		virtual bool CreateBoxActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, Vector3 halfExtend, float density) override;
		virtual bool CreateSphereActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, float radius, float density) override;
		virtual bool CreatePlainActor(const std::string& objName, const std::string& shapeName) override;
		virtual bool CreateCapsuleActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, float radius, float halfHeight, float density) override;
		virtual bool CreateTriangleMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, ePhysicsType phsicsType, Transform transform, float density) override;
		virtual bool CreateConvexMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, ePhysicsType phsicsType, Transform transform, float density) override;

		virtual bool DeleteActor(const std::string& objName) override;

		//액터 트리거화
		virtual bool SetTriggerShape(const std::string& objName, bool flag) override;

		//비활성화, 물리적인 판정, 레이캐스팅에서 제외됩니다.
		virtual bool DisableAcotr(const std::string& objName) override;
		virtual bool EnableAcotr(const std::string& objName) override;

		//현재 Scene의 객체의 쉐이프(Collider) 크기 제어
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

		//RayCast 시작점, 방향, 거리, outHit
		virtual bool Raycast(Vector3 origin, Vector3 dir, float distance, RayCastHit& hit) override;
		//RayCast 시작점, 방향, 거리, 필터링 될 레이어, 충돌할 오브젝트 타입, outHit;
		virtual bool Raycast(Vector3 origin, Vector3 dir, float distance, std::vector<std::string> Filteredlayers, ePhysicsType PhysicsFlags, RayCastHit& hit) override;

		//충돌레이어 지정을 위한 레이어 최대 31개까지 추가가능
		virtual bool AddLayer(const std::string& layerName) override;
		//FilteredLayer에 있는 이름과 같은 레이어는 충돌이 무시된다. 비용이 큼. 엔진 초기화에 추가하고, 마스킹 세팅할것. 
		//중간수정은 되나, 기존에 있던 오브젝트들에게 적용이 안될 수 있음. 버그발생 확률 매우높아짐
		virtual bool SetLayerFilterData(const std::string& layerName, std::vector<std::string> Filteredlayers) override;
		//오브젝트의 레이어를 설정한다. 오브젝트들의 defaultSetting은 모두 default이다.
		virtual bool SetLayer(const std::string& objName, const std::string& layerName) override;

		//이벤트
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

		//물리공간
		std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_currentScene;
	};
}

