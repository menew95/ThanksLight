#pragma once

#include "Component.h"

namespace TLGameEngine
{
class AudioSource;
class Text;

class Narration : public Component
{
public:
	Narration();
public:
	virtual void Awake() override;
	virtual void Update() override;

	void ReadLine(int num);
	void SetPlay(bool value) { m_isPlaying = value; }

	int GetLine() { return m_nextLine; }
	bool GetIsPlaying() { return m_isPlaying; }

	void AddSubtileText(std::string text);
private:
	std::shared_ptr<AudioSource> m_audioSource;
	int m_nextLine = 0;
	std::shared_ptr<Text> m_narration;

	std::vector<std::string> m_subtitleTexts;

	float m_subtitleInterval = 2.5f; // 시간이 없어서 만드는 미친 코드
	float m_currentSubtitleTime = 0;
	bool m_isEnd = false;
	int m_isPlaying = false;

	std::vector<FileInfo> m_Lines;
	FileID m_Subtitle;
	BOOST_DESCRIBE_CLASS(TLGameEngine::Narration, (), (), (), (m_Lines, m_isPlaying, m_Subtitle))
};
}
