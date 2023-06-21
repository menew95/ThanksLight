#include "pch.h"
#include "SSAO.h"
#include "Shader.h"
#include "GraphicsEngine.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "EngineCamera.h"

#include "ConstBufferManager.h"
#include "PostProcess.h"
#include "GraphicResourceManager.h"

TLGraphicsEngine::SSAO::SSAO()
:
m_Width(0), m_Height(0)
{
	
}

TLGraphicsEngine::SSAO::~SSAO()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_SSAORTV[i] != nullptr)
		{
			m_SSAORTV[i]->Release();
			delete m_SSAORTV[i];
			m_SSAORTV[i] = nullptr;
		}
	}
}

void TLGraphicsEngine::SSAO::Initialize()
{
	GraphicsEngine* engine = GraphicsEngine::Instance();

	//m_SSAOPS = engine->m_ResourceManager->Load<Shader>("SSAO_VS");
	m_SSAOVS = GraphicResourceManager::Instance()->GetShaderRef("SSAO_VS");
	m_SSAOPS = GraphicResourceManager::Instance()->GetShaderRef("SSAO_PS");
	
	m_SSAOBlurVS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_VS");
	m_SSAOBlurPS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_BlurPS");


	m_SSAORTV[0] = new RenderTargetView;
	m_SSAORTV[1] = new RenderTargetView;

	BuildRandomVectorTexture();
	BuildOffsetVectors();
}

void TLGraphicsEngine::SSAO::BuildFrustumFarCorners(float fovY, float farZ, float aspect)
{
	float halfHeight = farZ * tanf(0.5f * fovY);
	float halfWidth = aspect * halfHeight;

	m_FrustumFarCorners[0] = XMFLOAT4(-halfWidth, -halfHeight, farZ, 0.0f);
	m_FrustumFarCorners[1] = XMFLOAT4(-halfWidth, +halfHeight, farZ, 0.0f);
	m_FrustumFarCorners[2] = XMFLOAT4(+halfWidth, -halfHeight, farZ, 0.0f);
	m_FrustumFarCorners[3] = XMFLOAT4(+halfWidth, +halfHeight, farZ, 0.0f);
}

void TLGraphicsEngine::SSAO::BuildOffsetVectors()
{
	m_Offsets[0] = XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	m_Offsets[1] = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

	m_Offsets[2] = XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	m_Offsets[3] = XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

	m_Offsets[4] = XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	m_Offsets[5] = XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

	m_Offsets[6] = XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	m_Offsets[7] = XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

	m_Offsets[8] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_Offsets[9] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

	m_Offsets[10] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	m_Offsets[11] = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	m_Offsets[12] = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	m_Offsets[13] = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

	for (int i = 0; i < 14; i++)
	{
		float s = MathHelper::RandF(0.25f, 1.0f);

		XMVECTOR v = s * XMVector4Normalize(XMLoadFloat4(&m_Offsets[i]));

		XMStoreFloat4(&m_Offsets[i], v);
	}
}

void TLGraphicsEngine::SSAO::BuildRandomVectorTexture()
{
	ID3D11Device* device = GraphicsEngine::Instance()->GetDevice();

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 256;
	texDesc.Height = 256;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.SysMemPitch = 256 * sizeof(XMCOLOR);

	XMCOLOR* color = new XMCOLOR[256* 256];
	for (int i = 0; i < 256; ++i)
	{
		for (int j = 0; j < 256; ++j)
		{
			XMFLOAT3 v(MathHelper::RandF(), MathHelper::RandF(), MathHelper::RandF());

			color[i * 256 + j] = XMCOLOR(v.x, v.y, v.z, 0.0f);
		}
	}

	initData.pSysMem = color;

	ID3D11Texture2D* tex = nullptr;
	HR(device->CreateTexture2D(&texDesc, &initData, &tex));

	if (tex != nullptr)
	{
		HR(device->CreateShaderResourceView(tex, 0, &m_RandomVectorSRV));
	}

	// view saves a reference.
	ReleaseCOM(tex);

	delete[] color;
}

void TLGraphicsEngine::SSAO::Blur()
{
	GraphicsEngine* engine = GraphicsEngine::Instance();
	ID3D11DeviceContext* deviceContext = engine->GetDeviceContext();
	PostProcess* postProcess = engine->GetPostProcess();

	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);
	deviceContext->VSSetShader(m_SSAOBlurVS->GetShader<ID3D11VertexShader>(), nullptr, 0);
	postProcess->SetBlur(deviceContext, m_SSAORTV[0], m_SSAORTV[1]);
}

void TLGraphicsEngine::SSAO::OnResize(UINT width, UINT height)
{
	m_Width = width / 4;
	m_Height = height / 4;
	m_SSAORTV[0]->OnResize(m_Width, m_Height, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_SSAORTV[1]->OnResize(m_Width, m_Height, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void TLGraphicsEngine::SSAO::Setting(float fovY, float farZ, float aspect)
{
	BuildFrustumFarCorners(fovY, farZ, aspect);
	//BuildOffsetVectors();
}

void TLGraphicsEngine::SSAO::OnSSAO(/*ID3D11RenderTargetView* targetRTV, */ID3D11ShaderResourceView* normalSRV, ID3D11ShaderResourceView* depthSRV)
{
	GraphicsEngine* engine = GraphicsEngine::Instance();
	ID3D11DeviceContext* deviceContext = engine->GetDeviceContext();

	D3D11_VIEWPORT sv;
	sv.TopLeftX = 0;
	sv.TopLeftY = 0;
	sv.Width = (float)m_Width;
	sv.Height = (float)m_Height;
	sv.MinDepth = 0.f;
	sv.MaxDepth = 1.f;

	deviceContext->RSSetViewports(1, &sv);

	const float color[4] = { 1, 1, 1, 1 };
	deviceContext->ClearRenderTargetView(m_SSAORTV[0]->GetRTV(), &color[0]);

	deviceContext->OMSetRenderTargets(1, m_SSAORTV[0]->GetRTVR(), nullptr);

	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);

	// 렌더스테이트
	deviceContext->RSSetState(GraphicsEngine::Instance()->GetSolidRS());

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_SSAOVS->GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(QuadVertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &RenderTargetView::m_VB, &stride, &offset);
	deviceContext->IASetIndexBuffer(RenderTargetView::m_IB, DXGI_FORMAT_R32_UINT, 0);

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	static const XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);


	ID3D11ShaderResourceView** srvs[3] =
	{
		&normalSRV,
		&depthSRV,
		&m_RandomVectorSRV
	};

	deviceContext->VSSetShader(m_SSAOVS->GetShader<ID3D11VertexShader>(), nullptr, 0);
	deviceContext->PSSetShader(m_SSAOPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	deviceContext->DrawIndexed(6, 0, 0);

	// 이펙트 처리후에 사용된 RTV SRV 해제
	ID3D11ShaderResourceView* nullSRV[2] = { nullptr, nullptr };
	deviceContext->PSSetShaderResources(0, 2, &nullSRV[0]);
	ID3D11RenderTargetView* rtvNull = nullptr;
	deviceContext->OMSetRenderTargets(1, &rtvNull, nullptr);

	Blur();
}

ID3D11ShaderResourceView* TLGraphicsEngine::SSAO::GetSSAOMap()
{
	return m_SSAORTV[1]->GetSRV();
}

ID3D11ShaderResourceView** TLGraphicsEngine::SSAO::GetSSAOMapRef()
{
	return m_SSAORTV[1]->GetSRVR();
}
