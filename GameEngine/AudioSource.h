#pragma once

#include "Component.h"
#include "..\SoundLib\fmodcore_inc\fmod.hpp"

namespace TLGameEngine
{
	/// <summary>
	/// Audio Source를 재생 해줄 컴포넌트
	/// </summary>
	class AudioSource :
	    public Component
	{
	public:
		AudioSource();
		virtual ~AudioSource();
		static int Init();
		static int Release();

		virtual void Awake() override;
		virtual void Update() override;

		void SetClip(const UID& clip);
		bool GetLoop() { return Loop; }
		bool SetLoop(bool value) { Loop = value; }
		float GetVolume() { return m_Volume; }
		void SetVolume(float value);
		bool IsPlaying() { return m_isPlaying; }

		int play();
		int pause();
		int resume();
		int stop();

	private:
		void Load(const char* path, bool loop);

		static FMOD_SYSTEM* g_sound_system;

		FMOD_SOUND* m_sound;
		FMOD_CHANNEL* m_channel;

		FMOD_BOOL m_isPlaying;

		static float _bgmPlayTime;

		int Loop;
		float m_Volume;
		int m_PlayOnAwake;

		FileInfo m_audioClip;
		BOOST_DESCRIBE_CLASS(TLGameEngine::AudioSource, (), (), (), (Loop, m_PlayOnAwake, m_audioClip, m_Volume))
	};
}

