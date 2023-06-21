#include "pch.h"
#include "AudioSource.h"
#include "ResourceManager.h"

#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodL_vc.lib")

FMOD_SYSTEM* TLGameEngine::AudioSource::g_sound_system;

TLGameEngine::AudioSource::AudioSource()
	: Component(Component::Type::Action)
{

}

TLGameEngine::AudioSource::~AudioSource()
{
	stop();
}

int TLGameEngine::AudioSource::Init() {
	FMOD_System_Create(&g_sound_system, FMOD_VERSION);
	FMOD_System_Init(g_sound_system, 32, FMOD_INIT_NORMAL, nullptr);

	return 0;
}

int TLGameEngine::AudioSource::Release() {
	FMOD_System_Close(g_sound_system);
	FMOD_System_Release(g_sound_system);

	return 0;
}

void TLGameEngine::AudioSource::Awake()
{
	Load(ResourceManager::Instance().GetClipName(m_audioClip.guid).data(), Loop);


	if (m_PlayOnAwake)
	{
		play();
	}
}

void TLGameEngine::AudioSource::Update()
{
	FMOD_Channel_IsPlaying(m_channel, &m_isPlaying);

	if (m_isPlaying) {
		FMOD_System_Update(g_sound_system);
	}
}

void TLGameEngine::AudioSource::SetClip(const UID& clip)
{
	Load(ResourceManager::Instance().GetClipName(clip).data(), Loop);
}

void TLGameEngine::AudioSource::SetVolume(float value)
{
	m_Volume = value;
	FMOD_Channel_SetVolume(m_channel, m_Volume);
}

int TLGameEngine::AudioSource::play() {
	m_isPlaying = true;
	FMOD_System_PlaySound(g_sound_system, m_sound, nullptr, false, &m_channel);

	SetVolume(m_Volume);
	return 0;
}

int TLGameEngine::AudioSource::pause() {
	FMOD_Channel_SetPaused(m_channel, true);

	return 0;
}

int TLGameEngine::AudioSource::resume() {
	FMOD_Channel_SetPaused(m_channel, false);

	return 0;
}

int TLGameEngine::AudioSource::stop() {
	FMOD_Channel_Stop(m_channel);

	return 0;
}

void TLGameEngine::AudioSource::Load(const char* path, bool loop)
{
	std::string fullPath = path;
	std::string soundFilePath = "Resource/Sound/";
	size_t index = fullPath.find("Sounds");
	index += 7; // Sounds/ ¥Ÿ¿Ω ¿Œµ¶Ω∫
	soundFilePath += fullPath.substr(index, fullPath.size() - index);
	if (loop) {
		FMOD_RESULT result = FMOD_System_CreateSound(g_sound_system, soundFilePath.data(), FMOD_LOOP_NORMAL, 0, &m_sound);
		assert(result == FMOD_OK);
	}
	else {
		FMOD_RESULT result = FMOD_System_CreateSound(g_sound_system, soundFilePath.data(), FMOD_DEFAULT, 0, &m_sound);
		assert(result == FMOD_OK);
	}

	m_channel = nullptr;
}
