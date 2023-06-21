#pragma once
#include <windows.h>
#include "Export/ResourceBase.h"

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;
struct ID3D11InputLayout;
enum DXGI_FORMAT;

namespace TLGraphicsEngine
{
	class Shader;
	class MRTMaterial;

	struct QuadVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 UV;
	};

	/// <summary>
	///  RenderTargetView Class
	/// </summary>
	class RenderTargetView
	{
	public:
		RenderTargetView();
		~RenderTargetView();

		static void Init();
		static void Sets(RenderTargetView** targets, UINT count, class DepthStencilView* depthStencil);

	public:
		void Release();
		
		void OnResize(UINT width = 0, UINT height = 0, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

		void Set(class DepthStencilView* depthStencil);

	public:
		ID3D11RenderTargetView*		GetRTV()	{ return m_RTV; }
		ID3D11RenderTargetView**	GetRTVR()	{ return &m_RTV; }
		ID3D11ShaderResourceView*	GetSRV()	{ return m_SRV; }
		ID3D11ShaderResourceView**	GetSRVR()	{ return &m_SRV; }

		UINT GetWidth() { return m_Width; }
		UINT GetHeight() { return m_Height; }

	private:
		ID3D11RenderTargetView* m_RTV = nullptr;
		ID3D11ShaderResourceView* m_SRV = nullptr;

		DXGI_FORMAT					m_Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		ID3D11Texture2D* m_BackBuffer = nullptr;

		UINT m_Width = 0, m_Height = 0;

	public:
		// MRT를 하기 위한 멤버들
		static ID3D11Buffer* m_VB;
		static ID3D11Buffer* m_IB;

		static Shader* m_DeferredVS;
		static Shader* m_DeferredPS;
		static Shader* m_DeferredSSAOPS;

		static void Finialize();
	};
}

