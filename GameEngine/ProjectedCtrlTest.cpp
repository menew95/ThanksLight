#include "pch.h"
#include "ProjectedCtrlTest.h"
#include "SceneManager.h"
#include "Input.h"
#include "ProjectedRenderer.h"

TLGameEngine::ProjectedCtrlTest::ProjectedCtrlTest()
	: Component(Type::Action)
{

}

TLGameEngine::ProjectedCtrlTest::~ProjectedCtrlTest()
{

}

void TLGameEngine::ProjectedCtrlTest::Awake()
{
	m_projectedObject = SceneManager::Instance().FindObject(m_projectedObjectID);
}

void TLGameEngine::ProjectedCtrlTest::Update()
{
	if (Input::Instance().GetKeyDown(vk_alpha('b')))
	{
		m_projectedObject.lock()->GetComponent<ProjectedRenderer>()->SetRender2D();
	}
}
