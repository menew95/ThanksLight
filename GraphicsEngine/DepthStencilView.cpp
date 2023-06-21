#include "pch.h"
#include "DepthStencilView.h"
#include "GraphicsEngine.h"

TLGraphicsEngine::DepthStencilView::DepthStencilView()
{

}

TLGraphicsEngine::DepthStencilView::~DepthStencilView()
{
	
}

void TLGraphicsEngine::DepthStencilView::Release()
{
	ReleaseCOM(m_DSV);
	ReleaseCOM(m_BackBuffer);
	ReleaseCOM(m_SRV);
}

void TLGraphicsEngine::DepthStencilView::OnResize(UINT width /*= 0*/, UINT height /*= 0*/, bool bUseStencil /*= false*/)
{
	ReleaseCOM(m_DSV);
	ReleaseCOM(m_BackBuffer);
	ReleaseCOM(m_SRV);

	m_width = (width < 1) ? (UINT)GraphicsEngine::Instance()->GetClientWidth() : width;
	m_height = (height < 1) ? (UINT)GraphicsEngine::Instance()->GetClientHeight() : height;

	{
		// https://docs.microsoft.com/ko-kr/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#reading-the-depth-stencil-buffer-as-a-texture
		
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.Width = m_width;
		textureDesc.Height = m_height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = bUseStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		HR(GraphicsEngine::Instance()->GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_BackBuffer));
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = bUseStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		HR(GraphicsEngine::Instance()->GetDevice()->CreateDepthStencilView(m_BackBuffer, &desc, &m_DSV));
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = bUseStencil ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;
		HR(GraphicsEngine::Instance()->GetDevice()->CreateShaderResourceView(m_BackBuffer, &desc, &m_SRV));
	}
}
