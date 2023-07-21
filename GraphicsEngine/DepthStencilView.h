#pragma once
#include <windows.h>

struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

namespace TLGraphicsEngine
{
	
	/// <summary>
	///	DepthStencilView Class 
	/// </summary>
	
	class DepthStencilView
	{
	public:
		DepthStencilView();
		~DepthStencilView();

		void Release();

		void OnResize(UINT width = 0, UINT height = 0, bool bUseStencil = false);

	public:
		ID3D11ShaderResourceView* GetSRV()		{ return m_SRV; }
		ID3D11ShaderResourceView** GetSRVR()	{ return &m_SRV; }
		ID3D11DepthStencilView* GetDSV()		{ return m_DSV; }
		ID3D11DepthStencilView** GetDSVR()		{ return &m_DSV; }

		ID3D11Texture2D* GetBackBuffer()		{ return m_BackBuffer; }

		bool GetUseStencil() { return m_isUseStencil; }

		int GetWidth() { return m_width; }
		int GetHeight() { return m_height; }

	private:
		ID3D11ShaderResourceView* m_SRV = nullptr;
		ID3D11DepthStencilView* m_DSV = nullptr;
		ID3D11Texture2D* m_BackBuffer = nullptr;

		bool						m_isUseStencil = false;
		UINT						m_width = 0;
		UINT						m_height = 0;
	};
}

