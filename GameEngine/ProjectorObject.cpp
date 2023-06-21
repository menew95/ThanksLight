#include "pch.h"
#include "ProjectorObject.h"
#include "SceneManager.h"
#include "ProjectedRenderer.h"
#include "BoxCollider.h"

TLGameEngine::ProjectorObject::ProjectorObject()
	: Component(Type::Action)
{

}

TLGameEngine::ProjectorObject::~ProjectorObject()
{

}

void TLGameEngine::ProjectorObject::Awake()
{
	m_projectedRenderer = SceneManager::Instance().FindObject(m_primitiveObjectID)->GetComponent<ProjectedRenderer>();
	m_collider = GetComponent<BoxCollider>();

	m_gameObject.lock()->SetTag("ProjectedObject");
}

void TLGameEngine::ProjectorObject::Set2D()
{
	m_projectorOn = true;
	m_collider.lock()->SetIsActive(false);
	m_projectedRenderer.lock()->SetRender2D();
}
