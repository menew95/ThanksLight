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

		//일단 컨택트와 트리거만 ^^..
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		void CallonTriggerPersist();

		virtual bool AddColliderEvent(std::string eventHolder, IEventCollider*);
		virtual bool AddTriggerEvent(std::string eventHolder, IEventTrigger*);

		//대상 이벤트 홀더의 모든 이벤트를 삭제한다.
		void DeleteEvents(std::string eventHolder);
	
	private:
		std::weak_ptr<Scene> m_Scene;

		//void(타겟 오브젝트 이름)반환. 함수를 바인딩함. 이름과 똑같은 대상의 함수를 불러온다.
		std::unordered_map<std::string, std::vector<IEventCollider*>> m_ContactEvents;
		//트리거만 가능
		std::unordered_map<std::string, std::vector<IEventTrigger*>> m_TriggerEvents;
		std::unordered_map<std::string, std::list<std::string>> m_TriggerPersistEvents;


		std::mutex m_Mutex;

	};
}