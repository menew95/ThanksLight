#pragma once
#include <string>
#include <memory>

#include "../dllDefine.h"
#include "PhysicsInfo.h"

/// <summary>
/// 물리엔진 인터페이스
/// </summary>

namespace PhysicsEngine
{
	class PHYSICS_ENGINE IPhysicsEngine
	{
	protected:
		IPhysicsEngine() = default;
		virtual ~IPhysicsEngine() = default;

	public:
		virtual bool Initialize() abstract;
		virtual bool Finalalize() abstract;

		//Scene 관련 함수들
		virtual bool AddScene(const std::string& sceneName) abstract;
		virtual bool SetCurrentScene(const std::string& sceneName) abstract;
		virtual bool DeleteScene(const std::string& sceneName) abstract;

		//비활성화, 물리적인 판정, 레이캐스팅에서 제외됩니다.
		virtual bool DisableAcotr(const std::string& objName) abstract;
		virtual bool EnableAcotr(const std::string& objName) abstract;

		//현재 Scene에 객체 생성
		virtual bool CreateBoxActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, Vector3 halfExtend, float density) abstract;
		virtual bool CreateSphereActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, float radius, float density) abstract;
		virtual bool CreatePlainActor(const std::string& objName, const std::string& shapeName) abstract;
		virtual bool CreateCapsuleActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, float radius, float halfHeight, float density) abstract;
		virtual bool CreateTriangleMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, ePhysicsType phsicsType, Transform transform, float density) abstract;
		virtual bool CreateConvexMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, ePhysicsType phsicsType, Transform transform, float density) abstract;


		virtual bool DeleteActor(const std::string& objName) abstract;

		//액터 트리거화
		virtual bool SetTriggerShape(const std::string& objName, bool flag) abstract;

		//현재 Scene의 객체의 쉐이프(Collider) 크기 제어
		virtual bool SetBoxCollider(const std::string& objName, Vector3 halfExtend, float mass) abstract;

		//오브젝트 제어
		virtual bool AddForce(const std::string& objName, Vector3 vec) abstract;
		virtual bool AddImpulse(const std::string& objName, Vector3 force) abstract;
		virtual bool AddTorque(const std::string& objName, Vector3 force) abstract;
		virtual bool AddTorqueImpulse(const std::string& objName, Vector3 force) abstract;
		virtual bool SetLinearDamping(const std::string& objName, float damping) abstract;
		virtual bool SetAngularDamping(const std::string& objName, float damping) abstract;
		virtual bool SetLinearVelocity(const std::string& objName, Vector3 vel) abstract;
		virtual bool SetAnguarVelocity(const std::string& objName, Vector3 vel) abstract;
		
		virtual bool SetMaxLinearVelocity(const std::string& objName, float vel) abstract;
		virtual bool SetMaxAngularVelocity(const std::string& objName, float vel) abstract;

		virtual bool GetAnguarVelocity(const std::string& objName, Vector3& output) abstract;
		virtual bool GetLinearVelocity(const std::string& objName, Vector3& output) abstract;

		virtual bool SetTransform(const std::string& objName, Transform transform) abstract;
		virtual bool SetGravity(const std::string& objName, bool flag) abstract;
		virtual bool SetKinematic(const std::string& objName, bool flag) abstract;
		virtual bool SetMass(const std::string& objName, float mass) abstract;
		virtual bool SetAxisLock(const std::string& objName, PhysicsAixsLock lockInfo) abstract;

		//RayCast 시작점, 방향, 거리, outHit
		virtual bool Raycast(Vector3 origin, Vector3 dir, float distance, RayCastHit& hit) abstract;
		//RayCast 시작점, 방향, 거리, 필터링 될 레이어, 충돌할 오브젝트 타입, outHit;
		virtual bool Raycast(Vector3 origin, Vector3 dir, float distance, std::vector<std::string> Filteredlayers, ePhysicsType PhysicsFlags, RayCastHit& hit) abstract;

		//업데이트 함수.
		virtual bool Simualte(float deltaTime) abstract;

		//오브젝트 정보 가져오기
		virtual bool GetTransform(const std::string& objName, Transform& outputTrnasform) abstract;

		//충돌레이어 지정을 위한 레이어 최대 31개까지 추가가능
		virtual bool AddLayer(const std::string& layerName) abstract;
		//FilteredLayer에 있는 이름과 같은 레이어는 충돌이 무시된다. 비용이 큼. 엔진 초기화에 추가하고, 마스킹 세팅할것. 
		//중간수정은 되나, 기존에 있던 오브젝트들에게 적용이 안될 수 있음. 버그발생 확률 매우높아짐
		virtual bool SetLayerFilterData(const std::string& layerName, std::vector<std::string> Filteredlayers) abstract;
		//오브젝트의 레이어를 설정한다. 오브젝트들의 defaultSetting은 모두 default이다.
		virtual bool SetLayer(const std::string& objName, const std::string& layerName) abstract;

		//이벤트
		virtual bool AddColliderEvent(std::string eventHolder, IEventCollider*) abstract;
		virtual bool AddTriggerEvent(std::string eventHolder, IEventTrigger*)  abstract;

		virtual bool Test() abstract;

	};

	static std::shared_ptr<PhysicsEngine::IPhysicsEngine> g_physicsEngine = nullptr;


	PHYSICS_ENGINE std::shared_ptr<IPhysicsEngine> PhysicsEngineInstance();
	PHYSICS_ENGINE void PhysicsEngineRelease();
}
