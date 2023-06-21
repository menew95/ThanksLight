#pragma once
#include "Component.h"

/// <summary>
/// 레거시 애니메이션 컴포넌트
/// </summary>
namespace TLGameEngine 
{
	class AnimationClip;

	class Animation :
		public Component
	{
	public:
		Animation();
		virtual ~Animation();

	public:
		AnimationClip* m_CurrAnimationClip;

		bool m_IsPlaying;

		std::map<std::string, AnimationClip*> m_AnimationClipList;

	public:
		void AddClip(AnimationClip* clip, std::string clipName);
		bool Play();
		void Stop();
		void Next();
		void RemoveClip(std::string clipName);

	public:
		virtual void Init(GameObject* gameObject);

		virtual void PreUpdate();
		virtual void Update();
		virtual void LateUpdate();

		virtual void Destroy();

		virtual void OnPreRender();
		virtual void OnRender();
		virtual void OnPostRender();
	};
}

