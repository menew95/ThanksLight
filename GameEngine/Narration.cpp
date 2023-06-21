#include "pch.h"
#include "Narration.h"
#include "AudioSource.h"
#include "Text.h"
#include "SceneManager.h"

TLGameEngine::Narration::Narration()
	: Component(Type::Action)
{
}

void TLGameEngine::Narration::Awake()
{
	m_audioSource = m_gameObject.lock()->GetComponent<AudioSource>();
	assert(m_audioSource != nullptr);
	std::shared_ptr<GameObject> subtitleGameObject = SceneManager::Instance().FindObject(m_Subtitle);
	if (subtitleGameObject != nullptr)
	{
		m_narration = subtitleGameObject->GetComponent<Text>();
	}
}

void TLGameEngine::Narration::Update()
{
	if (m_isPlaying && !m_audioSource->IsPlaying() && m_nextLine < m_Lines.size())
	{
		if (m_narration != nullptr && m_nextLine < m_subtitleTexts.size())
		{
			m_narration->SetText(m_subtitleTexts[m_nextLine].data());
		}
		m_audioSource->SetClip(m_Lines[m_nextLine].guid);
		m_audioSource->play();
		m_nextLine++;
		if (m_nextLine >= m_Lines.size())
		{
			m_isPlaying = false;
		}
	}

	if (m_nextLine >= m_Lines.size() && m_nextLine < m_subtitleTexts.size())
	{
		m_currentSubtitleTime += Time::Instance().GetDeltaTime();

		if (m_currentSubtitleTime > m_subtitleInterval)
		{
			if (m_narration != nullptr && m_nextLine < m_subtitleTexts.size())
			{
				m_narration->SetText(m_subtitleTexts[m_nextLine].data());
			}
			else
			{
			}
			m_currentSubtitleTime -= m_subtitleInterval;
			m_nextLine++;
		}

		if (m_nextLine == m_subtitleTexts.size())
		{
			m_isEnd = true;
			m_currentSubtitleTime = 0;
		}
	}


	if (m_isEnd)
	{
		m_currentSubtitleTime += Time::Instance().GetDeltaTime();

		if (m_currentSubtitleTime > 3)
		{
			SceneManager::Instance().UnloadScene(SceneManager::Instance().GetCurrentSceneIndex());
			SceneManager::Instance().LoadScene(0);
		}

	}
}

void TLGameEngine::Narration::ReadLine(int num)
{
	assert(num >= 0 && num < m_Lines.size());
	
	m_nextLine = num;

	m_audioSource->SetClip(m_Lines[m_nextLine].guid);
	m_audioSource->play();
	m_nextLine++;
}

void TLGameEngine::Narration::AddSubtileText(std::string text)
{
	m_subtitleTexts.push_back(text);
}
