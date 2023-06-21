#pragma once
#include <string>

/// <summary>
/// ���� �Ŵ����� �߻�ȭ
/// 
/// 2022.02.19
/// </summary>
enum class IPlayMode
{
	BGM,
	Effect
};

namespace FMOD
{
	class Sound;
}

class ISoundManager abstract
{
public:
	virtual void Initialize() abstract;
	virtual void LoadSoundFile(std::string name, std::string filePath, unsigned int mode) abstract;
	virtual FMOD::Sound* CreateSoundFile(std::string name, std::string filePath, unsigned int mode) abstract;

	virtual void Update() abstract;
	virtual void Play(std::string name, IPlayMode mode) abstract;
	virtual void Play(FMOD::Sound* sound, IPlayMode mode) abstract;
	virtual void Delay(float delayTime, IPlayMode mode) abstract;

	virtual void Pause(IPlayMode mode, bool isPause) abstract;

	virtual void SetSoundSpeed(float speed, IPlayMode mode) abstract;
	virtual void SetSoundVoulume(float volume, IPlayMode mode) abstract;
	virtual void SetEffectSoundVoulume(float volume) abstract;

	virtual void Finalize() abstract;

};
