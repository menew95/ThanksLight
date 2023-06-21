#pragma once
#include <string>
#include <memory>

#include "../dllDefine.h"
#include "PhysicsInfo.h"

/// <summary>
/// �������� �������̽�
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

		//Scene ���� �Լ���
		virtual bool AddScene(const std::string& sceneName) abstract;
		virtual bool SetCurrentScene(const std::string& sceneName) abstract;
		virtual bool DeleteScene(const std::string& sceneName) abstract;

		//��Ȱ��ȭ, �������� ����, ����ĳ���ÿ��� ���ܵ˴ϴ�.
		virtual bool DisableAcotr(const std::string& objName) abstract;
		virtual bool EnableAcotr(const std::string& objName) abstract;

		//���� Scene�� ��ü ����
		virtual bool CreateBoxActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, Vector3 halfExtend, float density) abstract;
		virtual bool CreateSphereActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, float radius, float density) abstract;
		virtual bool CreatePlainActor(const std::string& objName, const std::string& shapeName) abstract;
		virtual bool CreateCapsuleActor(const std::string& objName, const std::string& shapeName, ePhysicsType physicstype, Transform transform, float radius, float halfHeight, float density) abstract;
		virtual bool CreateTriangleMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, ePhysicsType phsicsType, Transform transform, float density) abstract;
		virtual bool CreateConvexMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, ePhysicsType phsicsType, Transform transform, float density) abstract;


		virtual bool DeleteActor(const std::string& objName) abstract;

		//���� Ʈ����ȭ
		virtual bool SetTriggerShape(const std::string& objName, bool flag) abstract;

		//���� Scene�� ��ü�� ������(Collider) ũ�� ����
		virtual bool SetBoxCollider(const std::string& objName, Vector3 halfExtend, float mass) abstract;

		//������Ʈ ����
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

		//RayCast ������, ����, �Ÿ�, outHit
		virtual bool Raycast(Vector3 origin, Vector3 dir, float distance, RayCastHit& hit) abstract;
		//RayCast ������, ����, �Ÿ�, ���͸� �� ���̾�, �浹�� ������Ʈ Ÿ��, outHit;
		virtual bool Raycast(Vector3 origin, Vector3 dir, float distance, std::vector<std::string> Filteredlayers, ePhysicsType PhysicsFlags, RayCastHit& hit) abstract;

		//������Ʈ �Լ�.
		virtual bool Simualte(float deltaTime) abstract;

		//������Ʈ ���� ��������
		virtual bool GetTransform(const std::string& objName, Transform& outputTrnasform) abstract;

		//�浹���̾� ������ ���� ���̾� �ִ� 31������ �߰�����
		virtual bool AddLayer(const std::string& layerName) abstract;
		//FilteredLayer�� �ִ� �̸��� ���� ���̾�� �浹�� ���õȴ�. ����� ŭ. ���� �ʱ�ȭ�� �߰��ϰ�, ����ŷ �����Ұ�. 
		//�߰������� �ǳ�, ������ �ִ� ������Ʈ�鿡�� ������ �ȵ� �� ����. ���׹߻� Ȯ�� �ſ������
		virtual bool SetLayerFilterData(const std::string& layerName, std::vector<std::string> Filteredlayers) abstract;
		//������Ʈ�� ���̾ �����Ѵ�. ������Ʈ���� defaultSetting�� ��� default�̴�.
		virtual bool SetLayer(const std::string& objName, const std::string& layerName) abstract;

		//�̺�Ʈ
		virtual bool AddColliderEvent(std::string eventHolder, IEventCollider*) abstract;
		virtual bool AddTriggerEvent(std::string eventHolder, IEventTrigger*)  abstract;

		virtual bool Test() abstract;

	};

	static std::shared_ptr<PhysicsEngine::IPhysicsEngine> g_physicsEngine = nullptr;


	PHYSICS_ENGINE std::shared_ptr<IPhysicsEngine> PhysicsEngineInstance();
	PHYSICS_ENGINE void PhysicsEngineRelease();
}
