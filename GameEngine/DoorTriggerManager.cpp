#include "pch.h"
#include "DoorTriggerManager.h"
#include "Door.h"
#include "SceneManager.h"
#include "BoxCollider.h"

//트리거 종류
#include "ButtonTrigger.h"
#include "ButtonTrigger2.h"
#include "Level8ButtonTrigger.h"
#include "FillTrigger.h"
#include "DoorTrigger.h"


TLGameEngine::DoorTriggerManager::DoorTriggerManager()
{

}

TLGameEngine::DoorTriggerManager::~DoorTriggerManager()
{

}

void TLGameEngine::DoorTriggerManager::Awake()
{
	std::shared_ptr<Trigger> tempTrigger;

	if (m_nbOfTriggers > 0)
	{
		auto triggerObject = SceneManager::Instance().FindObject(m_Trigger1);
		tempTrigger = triggerObject->GetComponent<ButtonTrigger>();
		if (!tempTrigger)
		{
			tempTrigger = triggerObject->GetComponent<FillTrigger>();
			if (!tempTrigger)
			{
				tempTrigger = triggerObject->GetComponent<DoorTrigger>();
				if (!tempTrigger)
				{
					tempTrigger = triggerObject->GetComponent<ButtonTrigger2>();
					if (!tempTrigger)
					{
						tempTrigger = triggerObject->GetComponent<Level8ButtonTrigger>();
						if (!tempTrigger)
						{

							assert(false);
						}
					}
				}
			}
		}
		m_Triggers.emplace_back(tempTrigger);
	}

	if (m_nbOfTriggers > 1)
	{
		auto triggerObject = SceneManager::Instance().FindObject(m_Trigger2);
		tempTrigger = triggerObject->GetComponent<ButtonTrigger>();
		if (!tempTrigger)
		{
			tempTrigger = triggerObject->GetComponent<FillTrigger>();
			if (!tempTrigger)
			{
				tempTrigger = triggerObject->GetComponent<DoorTrigger>();
				if (!tempTrigger)
				{

					tempTrigger = triggerObject->GetComponent<ButtonTrigger2>();
					if (!tempTrigger)
					{

						assert(false);
					}
				}
			}
		}
		m_Triggers.emplace_back(tempTrigger);
	}

	if (m_nbOfTriggers > 2)
	{
		auto triggerObject = SceneManager::Instance().FindObject(m_Trigger3);
		tempTrigger = triggerObject->GetComponent<ButtonTrigger>();
		if (!tempTrigger)
		{
			tempTrigger = triggerObject->GetComponent<FillTrigger>();
			if (!tempTrigger)
			{
				tempTrigger = triggerObject->GetComponent<DoorTrigger>();
				if (!tempTrigger)
				{

					tempTrigger = triggerObject->GetComponent<ButtonTrigger2>();
					if (!tempTrigger)
					{

						assert(false);
					}
				}
			}
		}
		m_Triggers.emplace_back(tempTrigger);
	}

	if (m_nbOfTriggers > 3)
	{
		auto triggerObject = SceneManager::Instance().FindObject(m_Trigger4);
		tempTrigger = triggerObject->GetComponent<ButtonTrigger>();
		if (!tempTrigger)
		{
			tempTrigger = triggerObject->GetComponent<FillTrigger>();
			if (!tempTrigger)
			{
				tempTrigger = triggerObject->GetComponent<DoorTrigger>();
				if (!tempTrigger)
				{
					tempTrigger = triggerObject->GetComponent<ButtonTrigger2>();
					if (!tempTrigger)
					{

						assert(false);
					}
				}
			}
		}
		m_Triggers.emplace_back(tempTrigger);
	}

	if (m_nbOfTriggers > 4)
	{
		auto triggerObject = SceneManager::Instance().FindObject(m_Trigger5);
		tempTrigger = triggerObject->GetComponent<ButtonTrigger>();
		if (!tempTrigger)
		{
			tempTrigger = triggerObject->GetComponent<FillTrigger>();
			if (!tempTrigger)
			{
				tempTrigger = triggerObject->GetComponent<DoorTrigger>();
				if (!tempTrigger)
				{

					tempTrigger = triggerObject->GetComponent<ButtonTrigger2>();
					if (!tempTrigger)
					{

						assert(false);
					}
				}
			}
		}
		m_Triggers.emplace_back(tempTrigger);
	}

	m_Door = SceneManager::Instance().FindObject(m_DoorID)->GetComponent<Door>();
	//m_gameObject.lock()->GetComponent<BoxCollider>()->SetColliderLayer("TRIGGER");

}

void TLGameEngine::DoorTriggerManager::Update()
{
	bool result = true;

	for (auto& trigger : m_Triggers)
	{
		auto b = trigger.lock()->GetIsTrigger();
		result &= trigger.lock()->GetIsTrigger();
	}

	m_TriggersResult = result;
	m_Door.lock()->SetDoorEmissive(result);

	if (m_TriggersResult)
	{
		m_Door.lock()->SetDoorOpen(true);
	}
	else
	{
		m_Door.lock()->SetDoorOpen(false);
	}
}

void TLGameEngine::DoorTriggerManager::OnTriggerEnter(std::string name)
{
	m_IsTrigger = true;
}

void TLGameEngine::DoorTriggerManager::OnTriggerExit(std::string name)
{
	m_IsTrigger = false;
	m_Door.lock()->SetDoorOpen(false);
}

