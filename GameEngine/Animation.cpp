#include "pch.h"
#include "Animation.h"
#include "AnimationClip.h"

TLGameEngine::Animation::Animation()
: Component(Component::Type::Action)
{

}

TLGameEngine::Animation::~Animation()
{
	for (auto clip : m_AnimationClipList)
	{
		if (clip.second != nullptr)
		{
			delete clip.second;

			clip.second = nullptr;
		}
	}
}

void TLGameEngine::Animation::AddClip(AnimationClip* clip, std::string clipName)
{
	clip->Setting(m_gameObject.lock().get());
	m_AnimationClipList.insert({ clipName, clip });

	if (m_CurrAnimationClip == nullptr)
	{
		m_CurrAnimationClip = clip;
	}
}

bool TLGameEngine::Animation::Play()
{
	m_IsPlaying = !m_IsPlaying;

	return m_IsPlaying;
}

void TLGameEngine::Animation::Stop()
{
	m_IsPlaying = false;
}

void TLGameEngine::Animation::Next()
{
	m_IsPlaying = false;
	m_CurrAnimationClip->Next();
}

void TLGameEngine::Animation::RemoveClip(std::string clipName)
{
	auto clipIter = m_AnimationClipList.find(clipName);
	if (clipIter != m_AnimationClipList.end())
	{
		m_AnimationClipList.erase(clipIter);
	}
}

void TLGameEngine::Animation::Init(GameObject* gameObject)
{
	//Component::Init(gameObject);
}

void TLGameEngine::Animation::PreUpdate()
{

}

void TLGameEngine::Animation::Update()
{
	if (m_IsPlaying && m_CurrAnimationClip != nullptr)
	{
		m_CurrAnimationClip->Update();
	}
}

void TLGameEngine::Animation::LateUpdate()
{

}

void TLGameEngine::Animation::Destroy()
{

}

void TLGameEngine::Animation::OnPreRender()
{

}

void TLGameEngine::Animation::OnRender()
{

}

void TLGameEngine::Animation::OnPostRender()
{

}
