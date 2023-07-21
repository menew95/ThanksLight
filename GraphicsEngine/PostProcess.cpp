#include "pch.h"
#include "PostProcess.h"
#include "DX11Define.h"
#include "GraphicsEngine.h"
#include "GraphicResourceManager.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Shader.h"
#include "Mesh.h"
#include "EngineCamera.h"

#include "ConstBufferDataInfo.h"

TLGraphicsEngine::PostProcess::PostProcess()
{

}

TLGraphicsEngine::PostProcess::~PostProcess()
{
	for (size_t i = 0; i < m_DownSamplings.size(); i++)
	{
		if (m_DownSamplings[i] != nullptr)
		{
			m_DownSamplings[i]->Release();
			delete m_DownSamplings[i];
		}

		if (m_DownSamplings2[i] != nullptr)
		{
			m_DownSamplings2[i]->Release();
			delete m_DownSamplings2[i];
		}
	}

	if (m_OutLineRTV != nullptr)
	{
		m_OutLineRTV->Release();
		delete m_OutLineRTV;
	}

	if (m_OutLineDSV != nullptr)
	{
		m_OutLineDSV->Release();
		delete m_OutLineDSV;
	}
}

void TLGraphicsEngine::PostProcess::Initialize()
{
	m_PostProcessVS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_VS");
	m_PostProcessBlurPS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_Blur_PS");
	m_PostProcessBloomPS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_Bloom_PS");
	//m_PostProcessDOFPS = GraphicResourceManager::Instance()->GetResource<Shader>("PostProcess_DOF_PS");
	m_PostProcessToneMapPS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_ToneMap_PS");

	m_PostProcessOutLineVS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_OutLine_VS");
	m_PostProcessOutLinePS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_OutLine_PS");
	m_PostProcessOutLineAddPS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_OutLineAdd_PS");

	m_PostProcessDownPS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_DownSampling_PS");
	m_PostProcessUpPS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_UpSampling_PS");
	m_PostProcessAddPS = GraphicResourceManager::Instance()->GetShaderRef("PostProcess_AddSampling_PS");
	
	RenderTargetView* downSampleRTV;
	RenderTargetView* downSampleRTV2;

	int _width = GraphicsEngine::Instance()->GetClientWidth();
	int _height = GraphicsEngine::Instance()->GetClientHeight();

	m_OutLineRTV = new RenderTargetView;
	m_OutLineRTV->OnResize(_width, _height, DXGI_FORMAT_R8G8B8A8_UNORM);

	m_OutLineDSV = new DepthStencilView;
	m_OutLineDSV->OnResize(_width, _height, true);

	for (int i = 0; i < g_maxDownSampleCount; i++)
	{
		downSampleRTV = new RenderTargetView;
		downSampleRTV2 = new RenderTargetView;

		_width /= 2;
		_height /= 2;

		downSampleRTV->OnResize(_width, _height, DXGI_FORMAT_R32G32B32A32_FLOAT);
		downSampleRTV2->OnResize(_width, _height, DXGI_FORMAT_R32G32B32A32_FLOAT);

		m_DownSamplings.push_back(downSampleRTV);
		m_DownSamplings2.push_back(downSampleRTV2);
	}

}

TLGraphicsEngine::RenderTargetView* TLGraphicsEngine::PostProcess::PostProcessPass(RenderTargetView* srv, RenderTargetView* rtv)
{
	RenderTargetView* currSRV = srv;
	RenderTargetView* currRTV = rtv;

	if (m_bOnOffBlur)
	{
		GraphicsEngine::Instance()->BeginEvent(TEXT("Blur Pass"));

		Blur(currSRV, currRTV);

		std::swap(currSRV, currRTV);

		GraphicsEngine::Instance()->EndEvent();
	}

	if (m_bOnOffBloom)
	{
		GraphicsEngine::Instance()->BeginEvent(TEXT("Bloom Pass"));

		Bloom(currSRV, currRTV);

		std::swap(currSRV, currRTV);

		GraphicsEngine::Instance()->EndEvent();
	}

	if (m_bOnOffToneMapping)
	{
		GraphicsEngine::Instance()->BeginEvent(TEXT("Tone Map Pass"));
		ToneMap(currSRV, currRTV);

		std::swap(currSRV, currRTV);

		GraphicsEngine::Instance()->EndEvent();
	}

	AddOutLine(currSRV, currRTV);

	if (!m_bOnOffBlur && !m_bOnOffBloom && !m_bOnOffToneMapping)
	{
		currRTV = srv;
	}

	return currRTV;
}

void TLGraphicsEngine::PostProcess::Blur(RenderTargetView* srv, RenderTargetView* rtv)
{
	GraphicsEngine* engine = GraphicsEngine::Instance();
	ID3D11DeviceContext* deviceContext = engine->GetDeviceContext();

	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);
	deviceContext->VSSetShader(m_PostProcessVS->GetShader<ID3D11VertexShader>(), nullptr, 0);

	DownSampling(deviceContext, srv, m_DownSamplings[0]);
	SetBlur(deviceContext, m_DownSamplings[0], m_DownSamplings2[0]);

	for (int i = 0; i < g_maxDownSampleCount - 1; i++)
	{
		DownSampling(deviceContext, m_DownSamplings[i], m_DownSamplings[i + 1]);
		SetBlur(deviceContext, m_DownSamplings[i + 1], m_DownSamplings2[i + 1]);
	}

	int index;
	for (int i = g_maxDownSampleCount - 1; i > 0; i--)
	{
		index = i - 1;
		UpSampling(deviceContext, m_DownSamplings[i], m_DownSamplings[index], m_DownSamplings2[index], 0.5f, 0.5f);
	}

	UpSampling(deviceContext, m_DownSamplings2[0], srv, rtv, 0.5f, 0.5f);
}

void TLGraphicsEngine::PostProcess::Bloom(RenderTargetView* srv, RenderTargetView* rtv)
{
	// 다운 샘플링 + Bloom 필터
	GraphicsEngine* engine = GraphicsEngine::Instance();
	ID3D11DeviceContext* deviceContext = engine->GetDeviceContext();

	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);

	deviceContext->VSSetShader(m_PostProcessVS->GetShader<ID3D11VertexShader>(), nullptr, 0);

	BloomCurve(deviceContext, srv, m_DownSamplings[0]);

	// 최소까지 다운 샘플링
	for (int i = 0; i < g_maxDownSampleCount - 1; i++)
	{
		DownSampling(deviceContext, m_DownSamplings[i], m_DownSamplings[i + 1]);
	}

	// 다운 샘플링 된것을 블러 먹인후 업텍스처에 더함
	int index;
	for (int i = g_maxDownSampleCount - 1; i > 0; i--)
	{
		index = i - 1;

		SetBlur(deviceContext, m_DownSamplings[i], m_DownSamplings2[i]);

		UpSampling(deviceContext, m_DownSamplings[i], m_DownSamplings[index], m_DownSamplings2[index], 1.f, 1.f);

		std::swap(m_DownSamplings[index], m_DownSamplings2[index]);
	}

	// 다운 샘플링 된것을 블러 먹인후 업텍스처에 더함
	SetBlur(deviceContext, m_DownSamplings[0], m_DownSamplings2[0]);

	UpSampling(deviceContext, m_DownSamplings[0], srv, rtv, 1.f, 1.f);
}

void TLGraphicsEngine::PostProcess::ToneMap(RenderTargetView* srv, RenderTargetView* rtv)
{
	GraphicsEngine* engine = GraphicsEngine::Instance();
	ID3D11DeviceContext* deviceContext = engine->GetDeviceContext();

	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);
	deviceContext->VSSetShader(m_PostProcessVS->GetShader<ID3D11VertexShader>(), nullptr, 0);

	m_TargetRTV = rtv;

	PostProcess_ToneMapping_PSCB toneMappingCB;
	toneMappingCB.g_fHardExposure = m_fExposure;

	std::vector<CBBase*> cbs =
	{
		&toneMappingCB
	};

	ID3D11ShaderResourceView** srvs[] = { srv->GetSRVR() };

	m_PostProcessToneMapPS->Update(cbs.data(), srvs);
	deviceContext->PSSetShader(m_PostProcessToneMapPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	SetEffect(deviceContext);

	UnBindResource(deviceContext, 1, 1);
}

void TLGraphicsEngine::PostProcess::DOF(RenderTargetView* srv, RenderTargetView* rtv)
{
	assert(false);
}

void TLGraphicsEngine::PostProcess::OutLine(RenderTargetView* rtv)
{
	GraphicsEngine* engine = GraphicsEngine::Instance();
	ID3D11DeviceContext* deviceContext = engine->GetDeviceContext();
	EngineCamera* engineCamera = engine->GetEngineCamera();

	deviceContext->OMSetRenderTargets(1, m_OutLineRTV->GetRTVR(), nullptr);
	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);

	float _near = engineCamera->GetNear();
	float _far = engineCamera->GetFar();
	float _fdivn = _far / _near;

	PostProcess_OutLine_PSCB outLineCB;

	outLineCB.thickNess = 2;
	outLineCB.minDepth = 0;
	outLineCB.maxDepth = 1;
	outLineCB.mainTex_TexelSize = XMFLOAT2(1.f / m_OutLineRTV->GetWidth(), 1.f / m_OutLineRTV->GetHeight());
	outLineCB.orthoParams = XMFLOAT4(m_OutLineRTV->GetWidth(), m_OutLineRTV->GetHeight(), 0, 0);
	outLineCB.zBufferParams = XMFLOAT4(1 - _fdivn, _fdivn, 1 / _far, 1 / _near);
	outLineCB.projectionParams = XMFLOAT4(1, _near, _far, 1 / _far);

	std::vector<CBBase*> cbs =
	{
		&outLineCB
	};

	m_PostProcessOutLineVS->Update(cbs.data(), nullptr);
	deviceContext->VSSetShader(m_PostProcessOutLineVS->GetShader<ID3D11VertexShader>(), nullptr, 0);

	std::vector<ID3D11ShaderResourceView**> srvs =
	{
		m_OutLineDSV->GetSRVR()
	};

	m_PostProcessOutLinePS->Update(cbs.data(), srvs.data());
	deviceContext->PSSetShader(m_PostProcessOutLinePS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	m_TargetRTV = m_OutLineRTV;

	SetEffect(deviceContext);

	UnBindResource(deviceContext, 2, 1);
}

void TLGraphicsEngine::PostProcess::AddOutLine(RenderTargetView* srv, RenderTargetView* rtv)
{
	GraphicsEngine* engine = GraphicsEngine::Instance();
	ID3D11DeviceContext* deviceContext = engine->GetDeviceContext();
	EngineCamera* engineCamera = engine->GetEngineCamera();

	deviceContext->OMSetDepthStencilState(engine->GetOffDepthCheckDSS(), 0);
	deviceContext->VSSetShader(m_PostProcessVS->GetShader<ID3D11VertexShader>(), nullptr, 0);

	std::vector<ID3D11ShaderResourceView**> srvs =
	{
		srv->GetSRVR(),
		m_OutLineRTV->GetSRVR()
	};

	m_PostProcessOutLineAddPS->Update(nullptr, srvs.data());
	deviceContext->PSSetShader(m_PostProcessOutLineAddPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	m_TargetRTV = rtv;

	SetEffect(deviceContext);

	UnBindResource(deviceContext, 2, 1);
}

void TLGraphicsEngine::PostProcess::SetEffect(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetRenderTargets(1, m_TargetRTV->GetRTVR(), nullptr);

	// 렌더스테이트
	deviceContext->RSSetState(GraphicsEngine::Instance()->GetSolidRS());

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_PostProcessVS->GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(QuadVertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &RenderTargetView::m_VB, &stride, &offset);
	deviceContext->IASetIndexBuffer(RenderTargetView::m_IB, DXGI_FORMAT_R32_UINT, 0);

	m_PostProcessVS->Update(nullptr, nullptr);

	deviceContext->DrawIndexed(6, 0, 0);
}

void TLGraphicsEngine::PostProcess::OnResize(UINT width, UINT height)
{
	m_OutLineRTV->OnResize(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_OutLineDSV->OnResize(width, height, true);

	for (size_t i = 0; i < g_maxDownSampleCount; i++)
	{
		width /= 2;
		height /= 2;
		m_DownSamplings[i]->OnResize(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
		m_DownSamplings2[i]->OnResize(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	}
}

void TLGraphicsEngine::PostProcess::BloomCurve(ID3D11DeviceContext* deviceContext, RenderTargetView* srv, RenderTargetView* rtv)
{
	D3D11_VIEWPORT sv;
	sv.TopLeftX = 0;
	sv.TopLeftY = 0;
	sv.Width = (float)rtv->GetWidth();
	sv.Height = (float)rtv->GetHeight();
	sv.MinDepth = 0.f;
	sv.MaxDepth = 1.f;

	deviceContext->RSSetViewports(1, &sv);

	m_TargetRTV = rtv;

	PostProcess_Bloom_PSCB _bloomCB;
	_bloomCB.Threshold_Min = m_fThresholdMin;
	_bloomCB.Threshold_Max = m_fThresholdMax;

	std::vector<CBBase*> cbs =
	{
		&_bloomCB
	};

	ID3D11ShaderResourceView** srvs[] = { srv->GetSRVR() };

	m_PostProcessBloomPS->Update(cbs.data(), srvs);

	deviceContext->PSSetShader(m_PostProcessBloomPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	SetEffect(deviceContext);

	UnBindResource(deviceContext, 1, 1);
}

void TLGraphicsEngine::PostProcess::SetBlur(ID3D11DeviceContext* deviceContext, RenderTargetView* srv, RenderTargetView* rtv)
{
	deviceContext->PSSetShader(m_PostProcessBlurPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	SetBlurAxis(deviceContext, 0, srv, rtv);
	SetBlurAxis(deviceContext, 1, rtv, srv);
}

void TLGraphicsEngine::PostProcess::SetBlurAxis(ID3D11DeviceContext* deviceContext, int axis, RenderTargetView* srv, RenderTargetView* rtv)
{
	m_TargetRTV = rtv;

	PostProcess_Blur_PSCB blurCB;
	blurCB.BlurInfo.z = 1 / (float)srv->GetWidth();
	blurCB.BlurInfo.w = 1 / (float)srv->GetHeight();

	m_CurrEffect = m_PostProcessBlurPS.get();

	blurCB.BlurInfo.x = (float)axis;

	std::vector<CBBase*> cbs =
	{
		&blurCB
	};

	ID3D11ShaderResourceView** srvs[] = { srv->GetSRVR() };

	m_PostProcessBlurPS->Update(cbs.data(), srvs);

	SetEffect(deviceContext);

	UnBindResource(deviceContext, 1, 1);
}

void TLGraphicsEngine::PostProcess::DownSampling(ID3D11DeviceContext* deviceContext, RenderTargetView* srv, RenderTargetView* rtv)
{
	m_TargetRTV = rtv;

	m_CurrEffect = m_PostProcessDownPS.get();

	D3D11_VIEWPORT sv;
	sv.TopLeftX = 0;
	sv.TopLeftY = 0;
	sv.Width = (float)rtv->GetWidth();
	sv.Height = (float)rtv->GetHeight();
	sv.MinDepth = 0.f;
	sv.MaxDepth = 1.f;

	deviceContext->RSSetViewports(1, &sv);

	ID3D11ShaderResourceView** srvs[] = {
		srv->GetSRVR(),
	};

	m_PostProcessDownPS->Update(nullptr, srvs);

	deviceContext->PSSetShader(m_PostProcessDownPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	SetEffect(deviceContext);

	UnBindResource(deviceContext, 1, 1);
}

void TLGraphicsEngine::PostProcess::UpSampling(ID3D11DeviceContext* deviceContext, RenderTargetView* downSRV, RenderTargetView* upSRV, RenderTargetView* rtv, float blendDown, float blendUp)
{
	m_TargetRTV = rtv;

	D3D11_VIEWPORT sv;
	sv.TopLeftX = 0;
	sv.TopLeftY = 0;
	sv.Width = (float)rtv->GetWidth();
	sv.Height = (float)rtv->GetHeight();
	sv.MinDepth = 0.f;
	sv.MaxDepth = 1.f;

	deviceContext->RSSetViewports(1, &sv);

	m_CurrEffect = m_PostProcessUpPS.get();

	ID3D11ShaderResourceView** srvs[] = {
		downSRV->GetSRVR(),
		upSRV->GetSRVR()
	};

	PostProcess_Up_PSCB upCB;
	upCB.UpInfo.x = blendDown;
	upCB.UpInfo.y = blendUp;


	CBBase* cbs[1]{ &upCB };

	m_PostProcessUpPS->Update(cbs, srvs);

	deviceContext->PSSetShader(m_PostProcessUpPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	SetEffect(deviceContext);

	UnBindResource(deviceContext, 2, 1);
}

void TLGraphicsEngine::PostProcess::AddSampling(ID3D11DeviceContext* deviceContext, RenderTargetView* downSRV, RenderTargetView* upSRV, RenderTargetView* rtv)
{
	m_TargetRTV = rtv;

	D3D11_VIEWPORT sv;
	sv.TopLeftX = 0;
	sv.TopLeftY = 0;
	sv.Width = (float)rtv->GetWidth();
	sv.Height = (float)rtv->GetHeight();
	sv.MinDepth = 0.f;
	sv.MaxDepth = 1.f;

	deviceContext->RSSetViewports(1, &sv);

	m_CurrEffect = m_PostProcessAddPS.get();

	ID3D11ShaderResourceView** srvs[] = {
		downSRV->GetSRVR(),
		upSRV->GetSRVR()
	};

	m_PostProcessAddPS->Update(nullptr, srvs);

	deviceContext->PSSetShader(m_PostProcessAddPS->GetShader<ID3D11PixelShader>(), nullptr, 0);

	SetEffect(deviceContext);

	UnBindResource(deviceContext, 2, 1);
}

void TLGraphicsEngine::PostProcess::UnBindResource(ID3D11DeviceContext* deviceContext, int srvCnt, int rtvCnt)
{
	std::vector<ID3D11ShaderResourceView*> _nullSRV(srvCnt, nullptr);
	deviceContext->PSSetShaderResources(0, srvCnt, _nullSRV.data());

	std::vector<ID3D11RenderTargetView*> rtvNull(rtvCnt, nullptr);
	deviceContext->OMSetRenderTargets(rtvCnt, rtvNull.data(), nullptr);
}
