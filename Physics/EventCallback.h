#pragma once
#include "PxSimulationEventCallback.h"
#include <mutex>

namespace PhysicsEngine
{
	class Scene;

	class EventCallback : public physx::PxSimulationEventCallback
	{
	public:
		EventCallback() = default;
		~EventCallback();

	public:
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {};
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) {};
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) {};
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {};

		//�ϴ� ����Ʈ�� Ʈ���Ÿ� ^^..
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		void CallonTriggerPersist();

		virtual bool AddColliderEvent(std::string eventHolder, IEventCollider*);
		virtual bool AddTriggerEvent(std::string eventHolder, IEventTrigger*);

		//��� �̺�Ʈ Ȧ���� ��� �̺�Ʈ�� �����Ѵ�.
		void DeleteEvents(std::string eventHolder);
	
	private:
		std::weak_ptr<Scene> m_Scene;

		//void(Ÿ�� ������Ʈ �̸�)��ȯ. �Լ��� ���ε���. �̸��� �Ȱ��� ����� �Լ��� �ҷ��´�.
		std::unordered_map<std::string, std::vector<IEventCollider*>> m_ContactEvents;
		//Ʈ���Ÿ� ����
		std::unordered_map<std::string, std::vector<IEventTrigger*>> m_TriggerEvents;
		std::unordered_map<std::string, std::list<std::string>> m_TriggerPersistEvents;


		std::mutex m_Mutex;

	};
}