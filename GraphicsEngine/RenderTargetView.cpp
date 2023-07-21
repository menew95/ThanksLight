#include "pch.h"
#include "GraphicsEngine.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Shader.h"
#include "GraphicResourceManager.h"

ID3D11Buffer* TLGraphicsEngine::RenderTargetView::m_VB = 0;
ID3D11Buffer* TLGraphicsEngine::RenderTargetView::m_IB = 0;
TLGraphicsEngine::Shader* TLGraphicsEngine::RenderTargetView::m_DeferredVS = 0;
TLGraphicsEngine::Shader* TLGraphicsEngine::RenderTargetView::m_DeferredPS = 0;
TLGraphicsEngine::Shader* TLGraphicsEngine::RenderTargetView::m_DeferredSSAOPS = 0;

void TLGraphicsEngine::RenderTargetView::Finialize()
{
	ReleaseCOM(m_VB);
	ReleaseCOM(m_IB);
}

TLGraphicsEngine::RenderTargetView::RenderTargetView()
{

}

TLGraphicsEngine::RenderTargetView::~RenderTargetView()
{

}

void TLGraphicsEngine::RenderTargetView::Release()
{
	ReleaseCOM(m_BackBuffer);
	ReleaseCOM(m_RTV);
	ReleaseCOM(m_SRV);
}

void TLGraphicsEngine::RenderTargetView::OnResize(UINT width /*= 0*/, UINT height /*= 0*/, DXGI_FORMAT format /*= DXGI_FORMAT_R8G8B8A8_UNORM*/)
{
	ReleaseCOM(m_BackBuffer);
	ReleaseCOM(m_RTV);
	ReleaseCOM(m_SRV);

	m_Width = (width < 1) ? (UINT)GraphicsEngine::Instance()->GetClientWidth() : width;
	m_Height = (height < 1) ? (UINT)GraphicsEngine::Instance()->GetClientHeight() : height;
	m_Format = format;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = m_Width;
	textureDesc.Height = m_Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = m_Format;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.SampleDesc.Count = 1;
	HR(GraphicsEngine::Instance()->GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_BackBuffer));

#ifdef _DEBUG
	m_BackBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("RenderTargetView::m_BackBuffer") - 1, "RenderTargetView::m_BackBuffer");
#endif

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = m_Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	HR(GraphicsEngine::Instance()->GetDevice()->CreateRenderTargetView(m_BackBuffer, &rtvDesc, &m_RTV));
#ifdef _DEBUG
	m_RTV->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("RenderTargetView::m_RTV") - 1, "RenderTargetView::m_RTV");
#endif

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	//ZeroMemory(&rtvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = m_Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	HR(GraphicsEngine::Instance()->GetDevice()->CreateShaderResourceView(m_BackBuffer, &srvDesc, &m_SRV));
#ifdef _DEBUG
	m_SRV->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("RenderTargetView::m_SRV") - 1, "RenderTargetView::m_SRV");
#endif
}

void TLGraphicsEngine::RenderTargetView::Set(class DepthStencilView* depthStencil)
{
	GraphicsEngine::Instance()->GetDeviceContext()->OMSetRenderTargets(1, &m_RTV, depthStencil->GetDSV());

	const float color = 0x0000ff;

	GraphicsEngine::Instance()->GetDeviceContext()->ClearRenderTargetView(m_RTV, &color);
	GraphicsEngine::Instance()->GetDeviceContext()->ClearDepthStencilView(depthStencil->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void TLGraphicsEngine::RenderTargetView::Sets(RenderTargetView** targets, UINT count, class DepthStencilView* depthStencil)
{
	std::vector<ID3D11RenderTargetView*> rtvs;

	for (UINT i = 0; i < count; i++)
	{
		ID3D11RenderTargetView* rtv = targets[i]->GetRTV();
		rtvs.push_back(rtv);

		float color[4];
		switch (i)
		{
		case 0:
			color[0] = 0x999999;
			color[1] = 0x999999;
			color[2] = 0x999999;
			color[3] = 0x999999;
			break;
		case 1:
			color[0] = 0x000000;
			color[1] = 0x000000;
			color[2] = 0x000000;
			color[3] = 0x000000;
			break;
		case 2:
			color[0] = 0xffffff;
			color[1] = 0xffffff;
			color[2] = 0xffffff;
			color[3] = 0xffffff;
			break;
		case 3:
			color[0] = 0x000000;
			color[1] = 0x000000;
			color[2] = 0x000000;
			color[3] = 0x000000;
			break;
		case 4:
			color[0] = 0x000000;
			color[1] = 0x000000;
			color[2] = 0x000000;
			color[3] = 0xffffff;
			break;
		case 5:
			color[0] = 0xffffff;
			color[1] = 0xffffff;
			color[2] = 0xffffff;
			color[3] = 0xffffff;
			break;
		}

		GraphicsEngine::Instance()->GetDeviceContext()->ClearRenderTargetView(rtv, color);
	}

	GraphicsEngine::Instance()->GetDeviceContext()->ClearDepthStencilView(depthStencil->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
	GraphicsEngine::Instance()->GetDeviceContext()->OMSetRenderTargets(count, &rtvs[0], depthStencil->GetDSV());
}

void TLGraphicsEngine::RenderTargetView::Init()
{
	QuadVertex vertices[4];

	vertices[0].Pos = { -1.f, -1.f, 0.0f };	// Left Bottom
	vertices[1].Pos = { -1.f, +1.f, 0.0f };	// Left Top
	vertices[2].Pos = { +1.f, -1.f, 0.0f };	// Right Bottom
	vertices[3].Pos = { +1.f, +1.f, 0.0f };	// Right Top

	vertices[0].UV = { 0, 1 };
	vertices[1].UV = { 0, 0 };
	vertices[2].UV = { 1, 1 };
	vertices[3].UV = { 1, 0 };

	vertices[0].Normal = { 0, 0, 0 };
	vertices[1].Normal = { 1, 0, 0 };
	vertices[2].Normal = { 2, 0, 0 };
	vertices[3].Normal = { 3, 0, 0 };

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(QuadVertex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(GraphicsEngine::Instance()->GetDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));

	UINT triangles[6];

	triangles[0] = 0;
	triangles[1] = 3;
	triangles[2] = 2;
	triangles[3] = 0;
	triangles[4] = 1;
	triangles[5] = 3;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = triangles;
	HR(GraphicsEngine::Instance()->GetDevice()->CreateBuffer(&ibd, &iinitData, &m_IB));

	m_DeferredVS = GraphicResourceManager::Instance()->GetShader("Deferred_VS_Light");
	m_DeferredPS = GraphicResourceManager::Instance()->GetShader("Deferred_PS_Light");
	m_DeferredSSAOPS = GraphicResourceManager::Instance()->GetShader("Deferred_PS_SSAOLight");
}
