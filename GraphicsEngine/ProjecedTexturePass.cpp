#include "pch.h"
#include "ProjecedTexturePass.h"
#include "Texture.h"
#include "Shader.h"
#include "GraphicsEngine.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "RenderQueue.h"
#include "GraphicResourceManager.h"
#include "EngineCamera.h"
#include "ProjectedTexture.h"
#include "ConstBufferDataInfo.h"

TLGraphicsEngine::ProjecedTexturePass::ProjecedTexturePass()
{
	m_Texture_VS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_VS");
	m_Texture_PS = GraphicResourceManager::Instance()->GetShaderRef("Texture_PS");
	m_ProjectedPass_PS = GraphicResourceManager::Instance()->GetShaderRef("ProjectedPass_PS");
	m_MeshVS = GraphicResourceManager::Instance()->GetShaderRef("Deferred_VS_Mesh");
	m_Forward_Light_PS = GraphicResourceManager::Instance()->GetShaderRef("Forward_PS_Light");

	for (int i = 0; i < MAX_PROJECTED_TEXTURE; i++)
	{
		m_projectedTextures.push_back(std::shared_ptr<ProjectedTexture>(new ProjectedTexture));
	}
}

TLGraphicsEngine::ProjecedTexturePass::~ProjecedTexturePass()
{

}


void TLGraphicsEngine::ProjecedTexturePass::RecordDepth(RenderTargetView* depthBuffer)
{
	for (auto proj : m_projectedTextures)
	{
		if (proj->GetOnOff() && proj->IsDirty())
		{
			RecordDepthBuffer(depthBuffer, proj);
		}
	}
}

void TLGraphicsEngine::ProjecedTexturePass::RecordDepthBuffer(RenderTargetView* depthBuffer, std::shared_ptr<ProjectedTexture> projected)
{
	auto engine = GraphicsEngine::Instance();
	auto* deviceContext = engine->GetDeviceContext();

	projected->BindDepth(deviceContext);

	// 렌더스테이트
	deviceContext->RSSetState(engine->GetSolidRS());

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_Texture_VS->GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(QuadVertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &RenderTargetView::m_VB, &stride, &offset);
	deviceContext->IASetIndexBuffer(RenderTargetView::m_IB, DXGI_FORMAT_R32_UINT, 0);

	ID3D11ShaderResourceView** srvs[1] = { depthBuffer->GetSRVR() };

	m_Texture_PS->Update(nullptr, srvs);

	deviceContext->VSSetShader(m_Texture_VS->GetShader<ID3D11VertexShader>(), nullptr, 0);
	deviceContext->PSSetShader(m_Texture_PS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	deviceContext->DrawIndexed(6, 0, 0);

	projected->SetDirty(false);

	ID3D11RenderTargetView* rtvNull = nullptr;
	deviceContext->OMSetRenderTargets(1, &rtvNull, nullptr);

	ID3D11ShaderResourceView* nullSRV[1] = {};
	deviceContext->PSSetShaderResources(0, 1, nullSRV);
}

void TLGraphicsEngine::ProjecedTexturePass::MakeProjectedTexture(RenderObject* renderObject)
{
	for (auto proj : m_projectedTextures)
	{
		if (!proj->GetIsSetObject())
		{
			RenderToProjectedTexture(renderObject, proj);
			proj->SetIsSetObject(true);
			return;
		}
	}
	//assert(false);
	// 프로젝터 갯수의 최대 값을 넘겼음
}

void TLGraphicsEngine::ProjecedTexturePass::RenderToProjectedTexture(RenderObject* renderObject, std::shared_ptr<ProjectedTexture> projected)
{
	auto* engine = GraphicsEngine::Instance();
	auto* deviceContext = engine->GetDeviceContext();

	projected->BindRTV(deviceContext);

	projected->SetDirty(true);
	projected->SetOnOff(true);

	m_MeshVS->Bind(deviceContext);

	deviceContext->OMSetDepthStencilState(GraphicsEngine::Instance()->GetNormalDSS(), 0);

	// 렌더스테이트
	deviceContext->RSSetState(GraphicsEngine::Instance()->GetShadowMapRS());

	XMMATRIX world = XMLoadFloat4x4(&renderObject->m_World);
	XMMATRIX worldInv = XMMatrixInverse(nullptr, world);
	XMMATRIX worldView = world * engine->GetEngineCamera()->GetViewTM();
	XMMATRIX worldViewProj = worldView * engine->GetEngineCamera()->GetProjTM();
	static const XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	PerObject_CB perObjectCB;
	perObjectCB.world = renderObject->m_World;
	XMStoreFloat4x4(&perObjectCB.worldInvTranspose, worldInv);
	XMStoreFloat4x4(&perObjectCB.worldView, worldView);
	XMStoreFloat4x4(&perObjectCB.worldViewProj, worldViewProj);
	XMStoreFloat4x4(&perObjectCB.texTransform, T);

	std::vector<CBBase*> cbs = {
		&perObjectCB
	};

	m_MeshVS->Update(cbs.data(), nullptr);

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_MeshVS->GetInputLayout());


	deviceContext->VSSetShader(m_MeshVS->GetShader<ID3D11VertexShader>(), nullptr, 0);

	auto _indexBuffer = renderObject->m_Mesh->GetIndexBuffers();

	for (size_t i = 0; i < _indexBuffer.size(); i++)
	{
		if (_indexBuffer[i]->GetIndexCount() < 1)
		{
			continue;
		}

		Material* _material;
		if (i < renderObject->m_Mats.size())
		{
			_material = renderObject->m_Mats[i];
		}
		else
		{
			_material = renderObject->m_Mats.back();
		}

		renderObject->m_Mesh->Bind(deviceContext, (int)i);

		auto ps = _material->GetShader();

		if (ps == nullptr)
		{
			// 쉐이더가 설정이 안된 이상한 머테리얼이 들어옴
			assert(false);
			continue;
		}

		std::vector<ID3D11ShaderResourceView**> srvs;


		if (_material->m_SetAlbedoMap)
		{
			srvs.push_back(_material->m_mainTex->GetSRVR());
		}
		else
		{
			srvs.push_back(nullptr);
		}
		if (_material->m_SetBumpMap)
		{
			srvs.push_back(_material->m_bumpTex->GetSRVR());
		}
		else
		{
			srvs.push_back(nullptr);
		}

		Light_CB lightcb;
		engine->GetLightBuffer(&lightcb, srvs);

		PerMaterial_CB perMaterialCB;
		perMaterialCB.material.diffuse = _material->m_MaterialCB.Diffuse;
		perMaterialCB.material.emissive = _material->m_MaterialCB.Emissive;
		perMaterialCB.material.metallic = _material->m_fMetallic;
		perMaterialCB.material.roughness = _material->m_fRoughness;
		perMaterialCB.material.isUseAlbedo = _material->m_SetAlbedoMap;
		perMaterialCB.material.isUseBump = _material->m_SetBumpMap;

		std::vector<CBBase*> cbs = {
			&lightcb,
			&perMaterialCB
		};
		m_Forward_Light_PS->Update(cbs.data(), srvs);

		m_Forward_Light_PS->Bind(deviceContext);

		deviceContext->DrawIndexed(_indexBuffer[i]->GetIndexCount(), 0, 0);
	}
}

void TLGraphicsEngine::ProjecedTexturePass::SetPass(RenderTargetView* worldRTV, RenderTargetView* target)
{
	auto* deviceContext = GraphicsEngine::Instance()->GetDeviceContext();

	for (auto proj : m_projectedTextures)
	{
		if (proj->GetOnOff() && !proj->IsDirty())
		{
			RenderProjector(deviceContext, worldRTV, target, proj);
		}
	}
}

void TLGraphicsEngine::ProjecedTexturePass::RenderProjector(ID3D11DeviceContext* deviceContext, RenderTargetView* worldRTV, RenderTargetView* target, std::shared_ptr<ProjectedTexture> projected)
{
	D3D11_VIEWPORT sv;
	sv.TopLeftX = 0;
	sv.TopLeftY = 0;
	sv.Width = (FLOAT)target->GetWidth();
	sv.Height = (FLOAT)target->GetHeight();
	sv.MinDepth = 0.f;
	sv.MaxDepth = 1.f;

	deviceContext->RSSetViewports(1, &sv);

	deviceContext->OMSetRenderTargets(1, target->GetRTVR(), nullptr);

	deviceContext->OMSetDepthStencilState(GraphicsEngine::Instance()->GetOffDepthCheckDSS(), 0);

	// 렌더스테이트
	deviceContext->RSSetState(GraphicsEngine::Instance()->GetSolidRS());

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_Texture_VS->GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(QuadVertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &RenderTargetView::m_VB, &stride, &offset);
	deviceContext->IASetIndexBuffer(RenderTargetView::m_IB, DXGI_FORMAT_R32_UINT, 0);

	Projected_PSCB cb;
	cb.camViewProj = projected->GetMatrix();

	std::vector<CBBase*> cbs = {
		&cb
	};

	ID3D11ShaderResourceView** srvs[3] = 
	{ 
		worldRTV->GetSRVR(), 
		projected->GetRenderTargetView()->GetSRVR(),
		projected->GetDepthStencilView()->GetSRVR()
	};

	m_ProjectedPass_PS->Update(cbs.data(), srvs);

	deviceContext->VSSetShader(m_Texture_VS->GetShader<ID3D11VertexShader>(), nullptr, 0);
	deviceContext->PSSetShader(m_ProjectedPass_PS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	deviceContext->DrawIndexed(6, 0, 0);

	ID3D11RenderTargetView* rtvNull = nullptr;
	deviceContext->OMSetRenderTargets(1, &rtvNull, nullptr);
}

void TLGraphicsEngine::ProjecedTexturePass::Reset()
{
	for (auto proj : m_projectedTextures)
	{
		proj->Reset();
	}
}

void TLGraphicsEngine::ProjecedTexturePass::OnResize(int width, int height)
{
	for (auto proj : m_projectedTextures)
	{
		proj->OnResize(width, height);
	}
}
