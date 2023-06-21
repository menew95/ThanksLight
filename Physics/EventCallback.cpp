#include "pch.h"
#include "EventCallback.h"



PhysicsEngine::EventCallback::~EventCallback()
{
	m_ContactEvents.clear();
	m_TriggerEvents.clear();
	m_TriggerPersistEvents.clear();
}

void PhysicsEngine::EventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i < nbPairs; i++)
	{
		const physx::PxContactPair& contactPair = pairs[i];

		// 유저 데이터가 없으면 에러상황임.
		if (pairHeader.actors[0]->userData == nullptr || pairHeader.actors[1]->userData == nullptr)
		{
			return;
		}

		auto foundEvent0 = m_ContactEvents.find(((UserData*)pairHeader.actors[0]->userData)->m_name);
		auto foundEvent1 = m_ContactEvents.find(((UserData*)pairHeader.actors[1]->userData)->m_name);
		bool foundEventTrigger0 = false;
		bool foundEventTrigger1 = false;


		if(foundEvent0 != m_ContactEvents.end())
		{
			foundEventTrigger0 = true; 
		};
		if(foundEvent1 != m_ContactEvents.end())
		{
			foundEventTrigger1 = true;
		};
		
		if(foundEventTrigger0 == false && foundEventTrigger1 == false)
		{
			continue;
		}
		
		if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (foundEventTrigger0)
			{
				for (auto events : foundEvent0->second)
				{
					events->OnCollisionEnter(((UserData*)pairHeader.actors[1]->userData)->m_name);
				}
			}
			if (foundEventTrigger1)
			{
				for (auto events : foundEvent1->second)
				{
					events->OnCollisionEnter(((UserData*)pairHeader.actors[0]->userData)->m_name);
				}
			}
		}
		if(contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			if (foundEventTrigger0)
			{
				for (auto events : foundEvent0->second)
				{
					events->OnCollisionPersist(((UserData*)pairHeader.actors[1]->userData)->m_name);
				}
			}
			if (foundEventTrigger1)
			{
				for (auto events : foundEvent1->second)
				{
					events->OnCollisionPersist(((UserData*)pairHeader.actors[0]->userData)->m_name);
				}
			}
		}
		if(contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (foundEventTrigger0)
			{
				for (auto events : foundEvent0->second)
				{
					events->OnCollisionExit(((UserData*)pairHeader.actors[1]->userData)->m_name);
				}
			}
			if (foundEventTrigger1)
			{
				for (auto events : foundEvent1->second)
				{
					events->OnCollisionExit(((UserData*)pairHeader.actors[0]->userData)->m_name);
				}
			}
		}

	}
}

void PhysicsEngine::EventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i < count; i++)
	{
		// ignore pairs when shapes have been deleted
		if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
		{
			continue;
		}

		// 유저 데이터가 없으면 에러상황임.
		if (pairs[i].triggerActor->userData == nullptr || pairs[i].otherActor->userData == nullptr)
		{
			continue;
		}
		std::string triggerObject = ((UserData*)pairs[i].triggerActor->userData)->m_name;
		std::string hitObject = ((UserData*)pairs[i].otherActor->userData)->m_name;

		auto foundEvent = m_TriggerEvents.find(triggerObject);

		//해당 이벤트 없음
		if (m_TriggerEvents.end() == foundEvent)
		{
			continue;
		}
		else //이벤트 발견
		{
			for (auto triggerEvents : foundEvent->second)
			{
				if (physx::PxPairFlag::eNOTIFY_TOUCH_FOUND == pairs->status)
				{
					triggerEvents->OnTriggerEnter(hitObject);
					auto foundPersistEvent = m_TriggerPersistEvents.find(triggerObject);
					foundPersistEvent->second.push_back(hitObject);
				}

				if (physx::PxPairFlag::eNOTIFY_TOUCH_LOST == pairs->status)
				{
					triggerEvents->OnTriggerExit(hitObject);
					auto foundPersistEvent = m_TriggerPersistEvents.find(triggerObject);
					foundPersistEvent->second.remove(hitObject);
				}
			}
		}
	}
}

void PhysicsEngine::EventCallback::CallonTriggerPersist()
{
	for (auto& trigger : m_TriggerPersistEvents)
	{
		if (!trigger.second.empty())
		{
			auto foundEvent = m_TriggerEvents.find(trigger.first);
			
			//해당 이벤트 없음
			if (m_TriggerEvents.end() == foundEvent)
			{
				continue;
			}
			else //이벤트 발견
			{
			
				for (auto& triggerEvent : foundEvent->second)
				{
					for (auto& hitObjs : trigger.second)
					{
						triggerEvent->OnTriggerPersist(hitObjs);
					}
				}
			}
		}
	}
}

bool PhysicsEngine::EventCallback::AddColliderEvent(std::string eventHolder, IEventCollider* callBackClass)
{
	bool result = false;

	//m_Mutex.lock();
	auto foundEvent = m_ContactEvents.find(eventHolder);

	if (m_ContactEvents.end() == foundEvent)
	{
		//if (eventType == eColliderEventType::INVAILD)
		//{
		//}
		//else
		//{
		m_ContactEvents.emplace(eventHolder, std::vector<IEventCollider*>{callBackClass});
		result = true;
		//}
	}
	else
	{
		//if (eventType == eColliderEventType::INVAILD)
		//{
		//	//생성실패
		//}
		//else
		//{
		foundEvent->second.emplace_back(callBackClass);
		result = true;
		//}
	}
	//m_Mutex.unlock();

	return result;
}

bool PhysicsEngine::EventCallback::AddTriggerEvent(std::string eventHolder, IEventTrigger* callBackClass)
{
	bool result = false;

	//m_Mutex.lock();
	auto foundEvent = m_TriggerEvents.find(eventHolder);
	if (m_TriggerEvents.end() == foundEvent)
	{
		//if (eventType == eTriggerEventType::INVAILD)
		//{
		//	//생성실패
		//}
		//else
		//{
		m_TriggerEvents.emplace(eventHolder, std::vector<IEventTrigger*>{callBackClass});
		m_TriggerPersistEvents.emplace(eventHolder, std::list<std::string>());

		result = true;
		//}
	}
	else
	{
		//if (eventType == eTriggerEventType::INVAILD)
		//{
		//	//생성실패
		//}
		//else
		//{
		foundEvent->second.emplace_back(callBackClass);

		result = true;
		//}
	}
	//m_Mutex.unlock();

	return result;
}

void PhysicsEngine::EventCallback::DeleteEvents(std::string eventHolder)
{
	UINT foundTriggerEventCnt = 0;
	UINT foundContactEventCnt = 0;

	//m_Mutex.lock();

	m_ContactEvents.erase(eventHolder);
	m_TriggerEvents.erase(eventHolder);
	m_TriggerPersistEvents.erase(eventHolder);

	//m_Mutex.unlock();
}
