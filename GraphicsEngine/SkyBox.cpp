#include "pch.h"
#include "SkyBox.h"
#include "EngineCamera.h"
#include "GraphicsEngine.h"
#include "DebugMesh.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "GraphicResourceManager.h"
#include "ConstBufferManager.h"

TLGraphicsEngine::SkyBox::SkyBox(EngineCamera* engineCamera, Mesh* mesh, Material* material/*, Texture* texture*/)
: m_SkyBoxMesh(mesh)
, m_EngineCamera(engineCamera)
{

}

TLGraphicsEngine::SkyBox::SkyBox(EngineCamera* engineCamera)
: m_EngineCamera(engineCamera)
{

}

TLGraphicsEngine::SkyBox::~SkyBox()
{
	ReleaseCOM(m_SkyBoxSampler);
}

void TLGraphicsEngine::SkyBox::Initialize()
{
	//m_CubeMapMaterial = GraphicResourceManager::Instance()->GetResource<Material>("SkyBox");
	m_SkyBoxMesh = GraphicResourceManager::Instance()->GetMeshRef("Cube");
	m_SkyBoxVS = GraphicResourceManager::Instance()->GetShaderRef("SkyBox_VS");
	m_SkyBoxPS = GraphicResourceManager::Instance()->GetShaderRef("SkyBox_PS");
	m_SkyBoxTextrue = GraphicResourceManager::Instance()->GetTextureRef("SkyBox.dds");
	m_BRDFTextrue = GraphicResourceManager::Instance()->GetTextureRef("ibl_brdf_lut.dds");
	m_IrradianceTextrue = GraphicResourceManager::Instance()->GetTextureRef("Irradiance.dds");

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MinLOD = -FLT_MAX;

	GraphicsEngine::Instance()->GetDevice()->CreateSamplerState(&samplerDesc, &m_SkyBoxSampler);
}

void TLGraphicsEngine::SkyBox::Render()
{
	GraphicsEngine* engine = GraphicsEngine::Instance();
	auto deviceContext = engine->GetDeviceContext();
	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);

	// 렌더스테이트
	deviceContext->RSSetState(engine->GetNoCullModeRS());

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_SkyBoxVS->GetInputLayout());

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_SkyBoxMesh->Bind(deviceContext, 0);

	ID3D11ShaderResourceView** srvs[1] = { m_SkyBoxTextrue->GetSRVR() };

	m_SkyBoxPS->Update(nullptr, srvs);

	m_SkyBoxVS->Bind(deviceContext);
	m_SkyBoxPS->Bind(deviceContext);

	deviceContext->DrawIndexed(m_SkyBoxMesh->GetIndexBuffers()[0]->GetIndexCount(), 0, 0);
}