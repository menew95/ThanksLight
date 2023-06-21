#pragma once
#include <string>
#include <queue>
#include <Windows.h>

#include "..\DXTK\Inc\SpriteFont.h"
#include "..\DXTK\Inc\SpriteBatch.h"


namespace TLGraphicsEngine
{
	class Texture;

	enum class eImageType {
		Simple = 0,
		Filled = 1
	};

	struct TextObject
	{
		TextObject(DirectX::XMFLOAT2 pos, std::string text, DirectX::XMFLOAT4 color, int fontSize, float lineSpacing, DirectX::XMFLOAT2 alignment)
			: m_position(pos)
			, m_text(text)
			, m_color(color)
			, m_fontSize(fontSize)
			, m_value(lineSpacing)
			, m_alignment(alignment)
		{}

		DirectX::XMFLOAT2 m_position;
		std::string m_text;
		DirectX::XMFLOAT4 m_color;
		int m_fontSize;
		float m_value;
		DirectX::XMFLOAT2 m_alignment;
	};

	struct ImageObject
	{
		ImageObject(DirectX::XMFLOAT2 pos, TLGraphicsEngine::Texture* texture, DirectX::XMFLOAT4 color, RECT rect, int imageType, float value)
			: m_position(pos)
			, m_texture(texture)
			, m_color(color)
			, m_rect(rect)
			, m_eImageType(static_cast<eImageType>(imageType))
			, m_value(value)
		{}

		DirectX::XMFLOAT2 m_position;
		TLGraphicsEngine::Texture* m_texture;
		DirectX::XMFLOAT4 m_color;
		RECT m_rect;
		eImageType m_eImageType;
		float m_value;
	};

	enum class eUIType {
		Text = 0,
		Image = 1
	};

	struct UIObject
	{
		UIObject(const TextObject& textObject)
			: m_eType(eUIType::Text)
			, m_position(textObject.m_position)
			, m_text(textObject.m_text)
			, m_color(textObject.m_color)
			, m_iValue(textObject.m_fontSize)
			, m_fValue(textObject.m_value)
			, m_rect({ static_cast<LONG>(textObject.m_alignment.x * 100), static_cast<LONG>(textObject.m_alignment.y * 100), 0, 0 })
		{

		}

		UIObject(const ImageObject& imageObject)
			: m_eType(eUIType::Image)
			, m_position(imageObject.m_position)
			, m_texture(imageObject.m_texture)
			, m_color(imageObject.m_color)
			, m_rect(imageObject.m_rect)
			, m_eImageType(imageObject.m_eImageType)
			, m_fValue(imageObject.m_value)
		{

		}

		eUIType m_eType;

		DirectX::XMFLOAT2 m_position;
		DirectX::XMFLOAT4 m_color;

		// Text
		std::string m_text;

		// image
		TLGraphicsEngine::Texture* m_texture;
		RECT m_rect;
		eImageType m_eImageType;

		// Text : fontSize
		int m_iValue;

		// Text : lineSpacing
		// Image : value
		float m_fValue;
	};

	class UIRenderer
	{
	public:
		UIRenderer();
		~UIRenderer();

		void Create(ID3D11Device* pDevice, ID3D11RasterizerState* rs, ID3D11DepthStencilState* ds);

		void DrawTextColor(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text, ...);

		void RegistTextUI(DirectX::XMFLOAT2 pos, std::string text, DirectX::XMFLOAT4 color, int fontSize, float lineSpaceing, DirectX::XMFLOAT2 alignment);
		void RegistImageUI(DirectX::XMFLOAT2 pos, TLGraphicsEngine::Texture* texture, DirectX::XMFLOAT4 color, RECT rect, int imageType, float value);

		void BeginRender();
		void OnRender();
		void EndRender();

	private:
		void RenderText(UIObject* textObject);
		void RenderImage(UIObject* imageObject);
		void SetLineSpacing(float lineSpacing);

	private:
		std::queue<UIObject> m_uiQueue;

		DirectX::SpriteBatch* m_pSpriteBatch;
		DirectX::SpriteFont* m_pSpriteFont;

		// 그냥 사용하면 뎁스 문제가 생긴다.
		ID3D11RasterizerState* m_RasterizerState;
		ID3D11DepthStencilState* m_DepthStencilState;
	};
}