#include "pch.h"
#include "ProjectedTexture.h"
#include "GraphicsEngine.h"
#include "RenderTargetView.h"
#include "EngineCamera.h"

TLGraphicsEngine::ProjectedTexture::ProjectedTexture()
{
	m_renderTargetView = new RenderTargetView();
	m_depth = new RenderTargetView();
}

TLGraphicsEngine::ProjectedTexture::~ProjectedTexture()
{
	m_renderTargetView->Release();
	delete m_renderTargetView;
	m_depth->Release();
	delete m_depth;
}

void TLGraphicsEngine::ProjectedTexture::BindRTV(ID3D11DeviceContext* deviceContext)
{
	D3D11_VIEWPORT sv;
	sv.TopLeftX = 0;
	sv.TopLeftY = 0;
	sv.Width = (FLOAT)m_renderTargetView->GetWidth();
	sv.Height = (FLOAT)m_renderTargetView->GetHeight();
	sv.MinDepth = 0.f;
	sv.MaxDepth = 1.f;

	deviceContext->RSSetViewports(1, &sv);

	const float color[4] = {};
	deviceContext->ClearRenderTargetView(m_renderTargetView->GetRTV(), &color[0]);
	
	deviceContext->OMSetRenderTargets(1, m_renderTargetView->GetRTVR(), nullptr);

	XMStoreFloat4x4(&m_cameraViewProj, GraphicsEngine::Instance()->GetEngineCamera()->GetViewProjTM());
}

void TLGraphicsEngine::ProjectedTexture::BindDepth(ID3D11DeviceContext* deviceContext)
{
	D3D11_VIEWPORT sv;
	sv.TopLeftX = 0;
	sv.TopLeftY = 0;
	sv.Width = (FLOAT)m_depth->GetWidth();
	sv.Height = (FLOAT)m_depth->GetHeight();
	sv.MinDepth = 0.f;
	sv.MaxDepth = 1.f;

	deviceContext->RSSetViewports(1, &sv);

	const float color[4] = {};
	deviceContext->ClearRenderTargetView(m_depth->GetRTV(), &color[0]);

	deviceContext->OMSetRenderTargets(1, m_depth->GetRTVR(), nullptr);
}

void TLGraphicsEngine::ProjectedTexture::OnResize(int width, int height)
{
	m_renderTargetView->OnResize(width * 2, height * 2, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_depth->OnResize(width * 2, height * 2);
}

void TLGraphicsEngine::ProjectedTexture::Reset()
{
	m_isSetObject = false;
	m_isDirty = false;
	m_onOff = false;
}
