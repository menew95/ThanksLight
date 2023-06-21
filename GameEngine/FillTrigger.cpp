#include "pch.h"
#include "FillTrigger.h"
#include "SceneManager.h"
#include "InteractiveObject.h"
#include "Transform.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "MeshFilter.h"

TLGameEngine::FillTrigger::FillTrigger()
{

}

TLGameEngine::FillTrigger::~FillTrigger()
{

}

void TLGameEngine::FillTrigger::Awake()
{

	auto fillTrigger = SceneManager::Instance().FindObject(m_ForegroundTrigger);
	m_ForegroundMeshObj = SceneManager::Instance().FindObject(m_ForegroundMesh.fileID);

	if (!fillTrigger)
	{
		assert(false);
	}
	fillTrigger->GetComponent<BoxCollider>()->AddTriggerEvent(this);
}

void TLGameEngine::FillTrigger::Update()
{
	if (m_IsTrigger)
	{
		if (!m_OnceUpdate)
		{
			m_OnceUpdate = true;

			Vector3 TranshCan = { 0.f, 20.f, 100.f };

			m_ForegroundMeshObj.lock()->GetComponent<Transform>()->SetLocalPosition(TranshCan);
			m_CoreectObj.lock()->DropObject();
			m_CoreectObj.lock()->GetComponent<Transform>()->SetWorldPosition(TranshCan);
			//오브젝트 삭제 만들어줘!
		}
	}

}

void TLGameEngine::FillTrigger::OnTriggerEnter(std::string name)
{
	auto enterObject = SceneManager::Instance().FindObject(name);

	if (enterObject)
	{
		auto interactiveComp = enterObject->GetComponent<InteractiveObject>();

		if (interactiveComp)
		{
			auto keyType = interactiveComp->GetCurrent2DObjects();

			if (keyType)
			{
				auto KeyObject = interactiveComp->Get3DObjects();
				auto KeyObjectID = KeyObject->GetComponent<MeshFilter>();
				auto keyTypeID = keyType->GetComponent<MeshFilter>();

				bool why = keyTypeID->GetMeshUID() == m_KeyType.guid;
				bool trueee = (KeyObjectID->GetMeshUID() == m_KeyObject.guid);

				if (why & trueee)
				{
					m_IsTrigger = true;
					m_CoreectObj = interactiveComp;
				}
			}
		}
	}
}

void TLGameEngine::FillTrigger::OnTriggerExit(std::string name)
{

}

