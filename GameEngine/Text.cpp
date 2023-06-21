#include "pch.h"
#include "Text.h"
#include "RectTransform.h"
#include "GameEngine.h"
#include "IGraphicsEngine.h"
#include "Canvas.h"
#include "RectTransform.h"
#include "GameObject.h"

TLGameEngine::Text::Text() : Component(Type::Rendering)
{
}

TLGameEngine::Text::~Text()
{

}

void TLGameEngine::Text::Awake()
{
	m_rectTransform = GetComponent<RectTransform>();
	ConvertUnicode(m_Text);

	switch (static_cast<TLGameEngine::eAlignment>(m_FontData.m_Alignment))
	{
	case TLGameEngine::eAlignment::TopLeft:
		m_alignment = { 0, 1.0f };
		break;
	case TLGameEngine::eAlignment::TopMiddle:
		m_alignment = { 0.5f, 1.0f };
		break;
	case TLGameEngine::eAlignment::TopRight:
		m_alignment = { 1.0f, 1.0f };
		break;
	case TLGameEngine::eAlignment::MiddleLeft:
		m_alignment = { 0, 0.5f };
		break;
	case TLGameEngine::eAlignment::MiddleMiddle:
		m_alignment = { 0.5f, 0.5f };
		break;
	case TLGameEngine::eAlignment::MiddleRight:
		m_alignment = { 1.f, 0.5f };
		break;
	case TLGameEngine::eAlignment::BottomLeft:
		m_alignment = { 0, 0 };
		break;
	case TLGameEngine::eAlignment::BottomMiddle:
		m_alignment = { 0.5f, 0 };
		break;
	case TLGameEngine::eAlignment::BottomRight:
		m_alignment = { 1.f, 0 };
		break;
	default:
		break;
	}
}

void TLGameEngine::Text::LateUpdate()
{
	auto renderPos = m_rectTransform->GetLocalPosition();
	renderPos.y *= -1;

	renderPos.x += (float)Canvas::m_canvas->m_width * 0.5f; 
	renderPos.y += (float)Canvas::m_canvas->m_height * 0.5f;

	//renderPos.y -= m_rectTransform->GetRect().height * 0.5f;


	switch (static_cast<TLGameEngine::eAlignment>(m_FontData.m_Alignment))
	{
	case TLGameEngine::eAlignment::TopLeft:
		renderPos.x -= m_rectTransform->GetRect().width * 0.5f;
		m_alignment = { 0, 1.0f };
		break;
	case TLGameEngine::eAlignment::TopMiddle:
		m_alignment = { 0.5f, 1.0f };
		break;
	case TLGameEngine::eAlignment::TopRight:
		renderPos.x += m_rectTransform->GetRect().width * 0.5f;
		break;
	case TLGameEngine::eAlignment::MiddleLeft:
		renderPos.x -= m_rectTransform->GetRect().width * 0.5f;
		break;
	case TLGameEngine::eAlignment::MiddleMiddle:
		break;
	case TLGameEngine::eAlignment::MiddleRight:
		renderPos.x += m_rectTransform->GetRect().width * 0.5f;
		break;
	case TLGameEngine::eAlignment::BottomLeft:
		renderPos.x -= m_rectTransform->GetRect().width * 0.5f;
		m_alignment = { 0, 0 };
		break;
	case TLGameEngine::eAlignment::BottomMiddle:
		m_alignment = { 0.5f, 0 };
		break;
	case TLGameEngine::eAlignment::BottomRight:
		renderPos.x += m_rectTransform->GetRect().width * 0.5f;
		break;
	default:
		break;
	}

	GameEngine::Instance().GetGraphicsEngine()->RegistTextUIRenderQueue(renderPos.x, renderPos.y, m_Text.c_str(), &m_Color.r, m_FontData.m_FontSize, m_FontData.m_LineSpacing, m_alignment.x, m_alignment.y);
}

void TLGameEngine::Text::SetText(const char* text)
{
	m_Text = text;
}

std::wstring TLGameEngine::Text::ConvertUnicode(std::string& sourceText)
{
	size_t unicodePosition = sourceText.find("\\u");
	while (unicodePosition != std::string::npos)
	{
		const char* p = sourceText.c_str();
		p += unicodePosition + 2;
		int charSum = 0;
		for (int i = 0; i < 4; ++i)
		{
			if (*p >= '0' && *p <= '9')
			{
				charSum += (*p++) - '0';
			}
			else if (*p >= 'A' && *p <= 'F')
			{
				charSum += (*p++) - 'A' + 10;
			}

			if (i == 3) break;
			charSum = charSum << 4;
		}
		wchar_t wc[2] = { 0, };
		wc[0] = (wchar_t)charSum;
		char str[3];
		WideCharToMultiByte(CP_ACP, 0, wc, -1, str, 3, NULL, NULL);
		sourceText.erase(unicodePosition, 6);
		sourceText.insert(unicodePosition, str);
		unicodePosition = sourceText.find("\\u");
	}
	wchar_t wtext[1024] = { 0, };
	MultiByteToWideChar(CP_ACP, 0, sourceText.c_str(), (int)sourceText.length(), wtext, (int)sourceText.length());
	return wtext;
}