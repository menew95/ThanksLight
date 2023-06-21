#include "pch.h"
#include "AnimationClip.h"
#include "AnimationSnap.h"

TLGameEngine::AnimationClip::AnimationClip() 
:
	m_currFrameRate(0.f)
{
	
}

TLGameEngine::AnimationClip::~AnimationClip()
{
	for (auto snap : m_AnimationSnapList)
	{
		if (snap != nullptr)
		{
			delete snap;
			snap = nullptr;
		}
	}
}

void TLGameEngine::AnimationClip::Setting(GameObject* rootObject)
{

}

void TLGameEngine::AnimationClip::Next()
{
	for (auto clip : m_AnimationSnapList)
	{
		clip->Next();
	}
}

void TLGameEngine::AnimationClip::AddAnimationSnap(AnimationSnap* snap)
{
	m_AnimationSnapList.push_back(snap);
}

void TLGameEngine::AnimationClip::PreUpdate()
{

}

void TLGameEngine::AnimationClip::Update()
{
	//float delta = TLGameEngine::RGameEngine::Time->GetDeltaTime();
	//m_currFrameRate += delta;
	//float nextFrameRate = 0.f;
	//for (auto clip : m_AnimationSnapList)
	//{
	//	nextFrameRate = clip->Play(m_currFrameRate);
	//}
	//m_currFrameRate = nextFrameRate;
}

void TLGameEngine::AnimationClip::LateUpdate()
{

}

void TLGameEngine::AnimationClip::Destroy()
{

}

void TLGameEngine::AnimationClip::OnPreRender()
{

}

void TLGameEngine::AnimationClip::OnRender()
{

}

void TLGameEngine::AnimationClip::OnPostRender()
{

}
