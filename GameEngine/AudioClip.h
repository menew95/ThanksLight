#pragma once

#include "Object.h"

namespace FMOD
{
	class Sound;
}

namespace TLGameEngine
{
	class AudioClip : public Common::Object
	{
	public:
		AudioClip();
		virtual ~AudioClip();

	public:

		std::shared_ptr<FMOD::Sound> GetSound() { return m_soundData; }
		std::string GetName() { return m_name; }

	private:
		std::string m_name;
		std::shared_ptr<FMOD::Sound> m_soundData;
	};
}
