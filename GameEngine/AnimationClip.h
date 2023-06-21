#pragma once

namespace TLGameEngine
{
	class GameObject;
	class AnimationSnap;

	class AnimationClip
	{
	public:
		AnimationClip();
		virtual ~AnimationClip();

	public:
		std::vector<AnimationSnap*> m_AnimationSnapList;

		float m_currFrameRate;

	public:
		void Setting(GameObject* rootObject);
		void Next();
		void AddAnimationSnap(AnimationSnap* snap);
	public:
		virtual void PreUpdate();
		virtual void Update();
		virtual void LateUpdate();

		virtual void Destroy();

		virtual void OnPreRender();
		virtual void OnRender();
		virtual void OnPostRender();
	};
}

