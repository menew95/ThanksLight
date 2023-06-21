#pragma once
#include "Component.h"

struct FontData
{
public:
	std::wstring fontName;
	int m_FontSize = 50;
	FileInfo m_Font;
	int m_Alignment = 4;
	float m_LineSpacing = 1;
};
BOOST_DESCRIBE_STRUCT(FontData, (), (m_FontSize, m_Font, m_Alignment, m_LineSpacing))

namespace TLGameEngine
{
	enum class eAlignment
	{
		TopLeft			= 0,
		TopMiddle		= 1,
		TopRight		= 2,
		MiddleLeft		= 3,
		MiddleMiddle	= 4,
		MiddleRight		= 5,
		BottomLeft		= 6,
		BottomMiddle	= 7,
		BottomRight		= 8,
	};

	class RectTransform;

	class Text :
	    public Component
	{
	public:
		Text();
		virtual ~Text();

	public:
		virtual void Awake();
		virtual void OnEnable() {}
		virtual void OnDisalbe() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void LateUpdate();
		virtual void FixedUpdate() {}
		virtual void OnDestroy() {}

		void SetText(const char* text);

		void SetFontSize(int value) { m_FontData.m_FontSize = value; }
		void SetLineSpacing(float value) { m_FontData.m_LineSpacing = value; }

		std::wstring ConvertUnicode(std::string& sourceText);
	private:
		std::shared_ptr<RectTransform> m_rectTransform;
		Vector2 m_alignment;

		std::string m_Text;
		ParsingColor m_Color = { 1, 1, 1,1 };
		FontData m_FontData;

		BOOST_DESCRIBE_CLASS(Text, (), (), (), (m_Text, m_Color, m_FontData))
	};
}

