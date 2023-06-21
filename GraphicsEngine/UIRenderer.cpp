#include "UIRenderer.h"
#include <tchar.h>

#define SafeDelete(x) { delete x; x = 0; }
#define SAFE_RELEASE(x) { if(x){ x->Release(); x = 0; } }	// 예전 스타일

#include <DirectXMath.h>

#ifdef _DEBUG
	#ifndef x64
	#pragma comment(lib, "DirectXTK.lib")
	#else
	#pragma comment(lib, "DirectXTKx64Debug.lib")
	#endif // WIN32
#else
	#ifndef x64
	#pragma comment(lib, "DirectXTKr.lib")
	#else
	#pragma comment(lib, "DirectXTKx64Release.lib")
	#endif // WIN32
#endif // _DEBUG

#include "Texture.h"

using namespace TLGraphicsEngine;

#include "codecvt";

inline std::wstring s2ws(const std::string& txt)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(txt);
}
inline std::string ws2s(const std::wstring& txt)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(txt);
}

std::string w2s(const std::wstring& var)
{
	static std::locale loc("");
	auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
	return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).to_bytes(var);
}

std::wstring s2w(const std::string& var)
{
	static std::locale loc("");
	auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
	return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(var);
}

UIRenderer::UIRenderer()
	: m_pSpriteBatch(nullptr), m_pSpriteFont(m_pSpriteFont), m_RasterizerState(nullptr), m_DepthStencilState(nullptr)
{

}

UIRenderer::~UIRenderer()
{
	SafeDelete(m_pSpriteFont);
	SafeDelete(m_pSpriteBatch);
}

void UIRenderer::Create(ID3D11Device* pDevice, ID3D11RasterizerState* rs, ID3D11DepthStencilState* ds)
{
	ID3D11DeviceContext* pDC = nullptr;
	pDevice->GetImmediateContext(&pDC);

	m_pSpriteBatch = new DirectX::SpriteBatch(pDC);

	TCHAR* _fileName = (TCHAR*)L"Font/a고딕15.spritefont";

	m_pSpriteFont = new DirectX::SpriteFont(pDevice, _fileName);
	m_pSpriteFont->SetLineSpacing(14.0f);

	SAFE_RELEASE(pDC);

	m_RasterizerState = rs;
	m_DepthStencilState = ds;
}

void UIRenderer::DrawTextColor(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text, ...)
{
	TCHAR _buffer[1024] = L"";
	va_list vl;
	va_start(vl, text);
	_vstprintf(_buffer, 1024, text, vl);
	va_end(vl);
	DirectX::XMVECTOR c = XMLoadFloat4(&color);
	
	m_pSpriteBatch->Begin();
	m_pSpriteFont->DrawString(m_pSpriteBatch, _buffer, DirectX::XMFLOAT2((float)x, (float)y), c);
	m_pSpriteBatch->End();
}

void UIRenderer::SetLineSpacing(float lineSpacing)
{
	m_pSpriteFont->SetLineSpacing(lineSpacing);
}

void UIRenderer::RegistTextUI(DirectX::XMFLOAT2 pos, std::string text, DirectX::XMFLOAT4 color, int fontSize, float lineSpaceing, DirectX::XMFLOAT2 alignment)
{
	TextObject textObject
	{
		pos,
		text,
		color,
		fontSize,
		lineSpaceing,
		alignment
	};

	m_uiQueue.push(textObject);
}

void UIRenderer::RegistImageUI(DirectX::XMFLOAT2 pos, Texture* texture, DirectX::XMFLOAT4 color, RECT rect, int imageType, float value)
{
	ImageObject imageObject
	{
		pos,
		texture,
		color,
		rect,
		imageType,
		value
	};

	m_uiQueue.emplace(imageObject);
}

void UIRenderer::RenderText(UIObject* textObject)
{
	DirectX::XMVECTOR color = XMLoadFloat4(&textObject->m_color);


	m_pSpriteFont->SetLineSpacing((float)textObject->m_fValue);

	std::wstring temp = s2w(textObject->m_text);
	
	auto measure = m_pSpriteFont->MeasureString(temp.data());
	DirectX::XMFLOAT2 origin;
	DirectX::XMStoreFloat2(&origin, measure);

	origin.x *= textObject->m_rect.left / 100.f;
	origin.y *= textObject->m_rect.top / 100.f;

	m_pSpriteFont->DrawString(m_pSpriteBatch
	, temp.c_str()
	, textObject->m_position
	, color
	, 0
	, {origin}
	, textObject->m_iValue * 0.012f
	, DirectX::SpriteEffects_None
	, 0
	);
}

void UIRenderer::RenderImage(UIObject* imageObject)
{
	DirectX::XMVECTOR color = XMLoadFloat4(&imageObject->m_color);

	m_pSpriteBatch->Draw(imageObject->m_texture->GetSRV(), imageObject->m_rect, color);
}

void UIRenderer::BeginRender()
{
	m_pSpriteBatch->Begin(/*DirectX::SpriteSortMode_Deferred, nullptr, nullptr, m_DepthStencilState*/);
}

void UIRenderer::OnRender()
{
	while (!m_uiQueue.empty())
	{
		UIObject& currUIObject = m_uiQueue.front();

		switch (currUIObject.m_eType)
		{
			case eUIType::Text:
			{
				RenderText(&currUIObject);
				break;
			}
			case eUIType::Image:
			{
				RenderImage(&currUIObject);
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}

		m_uiQueue.pop();
	}
}

void UIRenderer::EndRender()
{
	m_pSpriteBatch->End();
}
