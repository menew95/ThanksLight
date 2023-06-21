#include "pch.h"
#include "AudioClip.h"

#include "..\SoundLib\fmodcore_inc\fmod.hpp"

void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

TLGameEngine::AudioClip::AudioClip()
{

}

TLGameEngine::AudioClip::~AudioClip()
{
	FMOD_RESULT result = m_soundData->release();
	ERRCHECK(result);
}
