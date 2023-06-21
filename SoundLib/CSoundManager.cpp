#include "CSoundManager.h"
#include "CommonDefine.h"
#include <windows.h>

#include <iostream>
#include <ostream>
#include <io.h>

#include "../GameEngine/MyTime.h"


/// <summary>
/// Create a System objectand initialize
/// </summary>
void CSoundManager::Initialize()
{
	//Common_Init(&extradriverdata);
	HRESULT _result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ERRCHECK(result);

	//AudioClip* _atk1 = ResourceManager::Instance()->GetAudioClip("Atk1");
	//result = system->createSound(_atk1->m_MetaFilePath.c_str(), FMOD_DEFAULT, 0, &sound_atk1);
	//ERRCHECK(result);
	/*std::string searching;
	searching += filePath;
	searching += soundFilter;
	_finddata_t fd;
	std::vector<std::string> filePaths;

	FindAllFileInDir(filePaths, searching, fd);

	if (!filePaths.empty())
	{
		for (auto& path : filePaths)
		{
			LoadSoundFile(path, filePath + path, );
		}
	}*/

	m_ElapsedTime = 0;
}

void CSoundManager::LoadSoundFile(std::string name, std::string filePath, unsigned int mode = 0)
{
	//FMOD_DEFAULT 0
	//FMOD_LOOP_OFF 1
	FMOD::Sound* _newSound = nullptr;
	result = system->createSound(filePath.c_str(), mode, 0, &_newSound);
	ERRCHECK(result);

	m_SoundList.insert({ name, _newSound });
}

FMOD::Sound* CSoundManager::CreateSoundFile(std::string name, std::string filePath, unsigned int mode)
{
	//FMOD_DEFAULT 0
	//FMOD_LOOP_OFF 1
	FMOD::Sound* _newSound = nullptr;
	result = system->createSound(filePath.c_str(), mode, 0, &_newSound);
	ERRCHECK(result);

	return _newSound;
}

void CSoundManager::Update()
{
	// 사운드시스템 업데이트가 특정 시간마다 일어나도록
	if (m_ElapsedTime < 3.0f)
	{
		m_ElapsedTime += TLGameEngine::Time::Instance().GetDeltaTime();
	}
	else
	{
		result = system->update();
		ERRCHECK(result);

		m_ElapsedTime = 0;
	}

	{
		unsigned int ms = 0;
		unsigned int lenms = 0;
		bool         playing = 0;
		bool         paused = 0;
		int          channelsplaying = 0;

		if (bgmChannel)
		{
			FMOD::Sound* currentsound = 0;

			result = bgmChannel->isPlaying(&playing);
			if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
			{
				ERRCHECK(result);
			}

			result = bgmChannel->getPaused(&paused);
			if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
			{
				ERRCHECK(result);
			}

			result = bgmChannel->getPosition(&ms, FMOD_TIMEUNIT_MS);
			if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
			{
				ERRCHECK(result);
			}

			bgmChannel->getCurrentSound(&currentsound);

			if (currentsound)
			{
				result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
				if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
				{
					ERRCHECK(result);
				}
			}
		}

		if (effectChannel)
		{
			FMOD::Sound* currentsound = 0;

			result = effectChannel->isPlaying(&playing);
			if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
			{
				ERRCHECK(result);
			}

			result = effectChannel->getPaused(&paused);
			if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
			{
				ERRCHECK(result);
			}

			result = effectChannel->getPosition(&ms, FMOD_TIMEUNIT_MS);
			if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
			{
				ERRCHECK(result);
			}

			effectChannel->getCurrentSound(&currentsound);

			if (currentsound)
			{
				result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
				if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
				{
					ERRCHECK(result);
				}
			}
		}

		system->getChannelsPlaying(&channelsplaying, NULL);
	}
}

void CSoundManager::Play(std::string name, IPlayMode mode)
{
	switch (mode)
	{
		case IPlayMode::BGM:
			PlayBGM(name);
			break;
		case IPlayMode::Effect:
			PlayEffect(name);
			break;
	}
}

void CSoundManager::Play(FMOD::Sound* sound, IPlayMode mode)
{
	switch (mode)
	{
	case IPlayMode::BGM:
		PlayBGM(sound);
		break;
	case IPlayMode::Effect:
		PlayEffect(sound);
		break;
	}
}

void CSoundManager::Delay(float delayTime, IPlayMode mode)
{
	// 찾아봐야 함
	//bgmChannel->setDelay()
	switch (mode)
	{
		case IPlayMode::BGM:
			bgmChannel->setDelay(1, 2);
			break;
		case IPlayMode::Effect:
			effectChannel->setDelay(1, (unsigned long)delayTime);
			break;
		default:
			break;
	}
}

void CSoundManager::Pause(IPlayMode mode, bool isPause)
{
	switch (mode)
	{
		case IPlayMode::BGM:
			bgmChannel->setPaused(isPause);
			break;
		case IPlayMode::Effect:
			effectChannel->setPaused(isPause);
			break;
		default:
			break;
	}
}

void CSoundManager::PlayBGM(std::string name)
{
	auto kvp = m_SoundList.find(name);
	if (kvp != m_SoundList.end())
	{
		bool isPlay = false;
		bgmChannel->isPlaying(&isPlay);
		if (isPlay)
		{
			bgmChannel->stop();
		}
		result = system->playSound(kvp->second, 0, false, &bgmChannel);
		ERRCHECK(result);
	}
}

void CSoundManager::PlayBGM(FMOD::Sound* sound)
{

	if (sound != nullptr)
	{
		bool isPlay = false;
		bgmChannel->isPlaying(&isPlay);
		if (isPlay)
		{
			bgmChannel->stop();
		}
		result = system->playSound(sound, 0, false, &bgmChannel);
		ERRCHECK(result);
	}
}

void CSoundManager::PlayEffect(std::string name)
{
	auto kvp = m_SoundList.find(name);
	if (kvp != m_SoundList.end())
	{
		result = system->playSound(kvp->second, 0, false, &effectChannel);
		effectChannel->setVolume(m_EffectSoundVolume);
		ERRCHECK(result);
	}
}

void CSoundManager::PlayEffect(FMOD::Sound* sound)
{
	if (sound != nullptr)
	{
		result = system->playSound(sound, 0, false, &effectChannel);
		effectChannel->setVolume(m_EffectSoundVolume);
		ERRCHECK(result);
	}
}

void CSoundManager::SetSoundSpeed(float speed, IPlayMode mode)
{
	switch (mode)
	{
		case IPlayMode::BGM:
			bgmChannel->setPitch(speed);
			break;
		case IPlayMode::Effect:
			effectChannel->setPitch(speed);
			break;
		default:
			break;
	}
}

void CSoundManager::SetSoundVoulume(float volume, IPlayMode mode)
{
	switch (mode)
	{
		case IPlayMode::BGM:
			bgmChannel->setVolume(volume);
			break;
		case IPlayMode::Effect:
			m_EffectSoundVolume = volume;
			break;
		default:
			break;
	}
}

void CSoundManager::SetEffectSoundVoulume(float volume)
{
	effectChannel->setVolume(m_ElapsedTime * volume);
}

/// <summary>
/// Shut down Sounds
/// </summary>
void CSoundManager::Finalize()
{
	for (auto _fmodSound : m_SoundList)
	{
		result = _fmodSound.second->release();
		ERRCHECK(result);
	}

	result = system->close();
	ERRCHECK(result);

	result = system->release();
	ERRCHECK(result);

	//Common_Close();
	CoUninitialize();
}

void CSoundManager::FindAllFileInDir(std::vector<std::string>& paths, const std::string& searching, _finddata64i32_t& fd)
{
	intptr_t handle = _findfirst(searching.c_str(), &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)    return;

	int result = 0;
	do
	{
		paths.push_back(fd.name);
		result = _findnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
}
