#include "pch.h"
#include "IBL.h"
#include "GraphicsEngine.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderTargetView.h"

using namespace TLGraphicsEngine;

TLGraphicsEngine::IBL::IBL()
{

}

TLGraphicsEngine::IBL::~IBL()
{

}

bool TLGraphicsEngine::IBL::CreatePregilteredMapSRV()
{
	HRESULT hr;

	ID3D11Texture2D* texture = nullptr;
	ID3D11RenderTargetView* m_RTV = nullptr;
	ID3D11ShaderResourceView* m_SRV = nullptr;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = m_iSkyBoxSize;
	textureDesc.Height = m_iSkyBoxSize;
	textureDesc.MipLevels = 5;
	textureDesc.ArraySize = 6;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	hr = GraphicsEngine::Instance()->GetDevice()->CreateTexture2D(&textureDesc, NULL, &texture);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.ArraySize = 6;
	rtvDesc.Texture2DArray.MipSlice = 5;

	hr = GraphicsEngine::Instance()->GetDevice()->CreateRenderTargetView(texture, &rtvDesc, &m_RTV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2D.MipLevels = 5;

	hr = GraphicsEngine::Instance()->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &m_SRV);

	if (SUCCEEDED(hr))
		return true;
	else
		return false;
}

bool TLGraphicsEngine::IBL::CreateIrradianceSRV()
{
	HRESULT hr;

	ID3D11Texture2D* texture = nullptr;
	ID3D11RenderTargetView* m_RTV = nullptr;
	ID3D11ShaderResourceView* m_SRV = nullptr;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = m_iSkyBoxSize;
	textureDesc.Height = m_iSkyBoxSize;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 6;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hr = GraphicsEngine::Instance()->GetDevice()->CreateTexture2D(&textureDesc, NULL, &texture);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;

	hr = GraphicsEngine::Instance()->GetDevice()->CreateRenderTargetView(texture, &rtvDesc, &m_RTV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2D.MipLevels = 1;
	
	hr = GraphicsEngine::Instance()->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &m_SRV);

	if (SUCCEEDED(hr))
		return true;
	else
		return false;
}

bool TLGraphicsEngine::IBL::CreatePregilteredMap(Texture* skybox)
{
	bool ret = false;

	GraphicsEngine* engine = GraphicsEngine::Instance();
	auto deviceContext = engine->GetDeviceContext();
	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);

	// 렌더스테이트
	deviceContext->RSSetState(engine->GetNoCullModeRS());

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_IBL_PrefilteredShaderVS->GetInputLayout());

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	m_SkyBoxMesh->Bind(deviceContext, 0);


	deviceContext->VSSetShader(m_IBL_PrefilteredShaderVS->GetShader<ID3D11VertexShader>(), nullptr, 0);
	deviceContext->PSSetShader(m_IBL_PrefilteredShaderPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	deviceContext->DrawIndexed(static_cast<int>(m_SkyBoxMesh->m_Triangles.size()), 0, 0);
	return ret;
}

bool TLGraphicsEngine::IBL::CreateBRDFMap()
{
	bool ret = false;

	GraphicsEngine* engine = GraphicsEngine::Instance();
	auto deviceContext = engine->GetDeviceContext();
	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);

	// 렌더스테이트
	deviceContext->RSSetState(engine->GetNoCullModeRS());

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_IBL_BRDFShaderVS->GetInputLayout());

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(QuadVertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_SkyBoxMesh->m_VB, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_SkyBoxMesh->m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	//m_IBL_BRDFShaderVS->Update();
	//m_IBL_BRDFShaderPS->Update();

	deviceContext->VSSetShader(m_IBL_BRDFShaderVS->GetShader<ID3D11VertexShader>(), nullptr, 0);
	deviceContext->PSSetShader(m_IBL_BRDFShaderPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	deviceContext->DrawIndexed(static_cast<int>(m_SkyBoxMesh->m_Triangles.size()), 0, 0);

	return ret;
}

bool TLGraphicsEngine::IBL::CreateIrradianceMap(Texture* skybox)
{
	bool ret = false;

	GraphicsEngine* engine = GraphicsEngine::Instance();
	auto deviceContext = engine->GetDeviceContext();
	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);

	// 렌더스테이트
	deviceContext->RSSetState(engine->GetNoCullModeRS());

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_IBL_IrradianceShaderVS->GetInputLayout());

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(QuadVertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_SkyBoxMesh->m_VB, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_SkyBoxMesh->m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Todo : 스카이 박스 셋
	//m_IBL_IrradianceShaderVS->Update();
	//m_IBL_IrradianceShaderPS->Update();

	deviceContext->VSSetShader(m_IBL_IrradianceShaderVS->GetShader<ID3D11VertexShader>(), nullptr, 0);
	deviceContext->PSSetShader(m_IBL_IrradianceShaderPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	deviceContext->DrawIndexed(static_cast<int>(m_SkyBoxMesh->m_Triangles.size()), 0, 0);

	return ret;
}

bool TLGraphicsEngine::IBL::IsDirty()
{
	return m_bIsDirty;
}

bool TLGraphicsEngine::IBL::SetIBL(Texture* skyBox)
{
	if (!m_bIsDirty)
	{
		return true;
	}

	if (!CreatePregilteredMap(skyBox))
	{
		return false;
	}
	if(!CreateBRDFMap())
	{
		return false;
	}
	if(!CreateIrradianceMap(skyBox))
	{
		return false;
	}

	return true;
}

TLGraphicsEngine::Texture* TLGraphicsEngine::IBL::GetPregilteredMap()
{
	return m_PrefilteredTextrue.get();
}

TLGraphicsEngine::Texture* TLGraphicsEngine::IBL::GetBRDFMap()
{
	return m_BRDFTextrue.get();
}

TLGraphicsEngine::Texture* TLGraphicsEngine::IBL::GetIrradianceMap()
{
	return m_IrradianceTextrue.get();
}
