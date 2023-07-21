#include "pch.h"
#include "GraphicsEngine.h"
#include "EngineCamera.h"
#include "UIRenderer.h"

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

#include "Texture.h"
#include "Parser/ParsingData.h"
#include "GraphicResourceManager.h"

#include "RenderQueue.h"
#include "Renderer.h"
#include "Frustum.h"
#include "SkyBox.h"

#include "ProjecedTexturePass.h"
#include "PostProcess.h"
#include "SSAO.h"

#include "LightManager.h"
//MRT
#include "RenderTargetView.h"
#include "DepthStencilView.h"

#include "ConstBufferManager.h"
#include "ConstBuffer.h"
#include "ConstBufferDataInfo.h"

#include "SamplerState.h"

constexpr float PI = 3.14159265359f;

TLGraphicsEngine::GraphicsEngine* TLGraphicsEngine::GraphicsEngine::m_graphicInstance = nullptr;

TLGraphicsEngine::GraphicsEngine::GraphicsEngine()
: m_fTheta(1.5f * PI)
, m_fPhi(0.25f * PI)
, m_fRadius(5.0f)
, m_isGraphicEnginePause(false)
, m_isMinimized(false)
, m_isMaximized(false)
, m_isResizing(false)
, m_pD3dDevice(nullptr)
, m_pD3dImmediateContext(nullptr)
, m_pSwapChain(nullptr)
, m_pRenderTargetView(nullptr)
, m_pDepthStencilView(nullptr)
, m_bEnable4xMsaa(false)
, m_pRenderQueue(nullptr)
, m_pFrustum(nullptr)
, m_pSkyBox(nullptr)
, m_pPostProcess(nullptr)
, m_pSSAO(nullptr)
, m_pLightManager(nullptr)
, m_bOnOffSSAO(false)
, m_iDrawObjCnt(0)
, m_pWireframeRS(nullptr)
, m_pSolidRS(nullptr)
, m_pNoCullModeRS(nullptr)
, m_pShadowMapRS(nullptr)
, m_NormalDSS(nullptr)
, m_Normal_StencilDSS(nullptr)
, m_Normal_Equal_StencilDSS(nullptr)
, m_ShadowDSS(nullptr)
, m_OffDepthCheckDSS(nullptr)
, m_MRTDSS(nullptr)
, m_HInstance(0)
, m_Hwnd(0)
, m_DirectX11Ready(S_OK)
, m_ClientWidth(0)
, m_ClientHeight(0)
, m_EngineCamera(nullptr)
, m_ResourceManager(nullptr)
, m_PickingCnt(0)
, m_NDCMousePosX(0.f)
, m_NDCMousePosY(0.f)
{

}

TLGraphicsEngine::GraphicsEngine::~GraphicsEngine()
{

}

TLGraphicsEngine::GraphicsEngine* TLGraphicsEngine::GraphicsEngine::Instance()
{
	if (m_graphicInstance == nullptr)
	{
		m_graphicInstance = new GraphicsEngine;
	}

	return m_graphicInstance;
}

void TLGraphicsEngine::GraphicsEngine::Finalize()
{

	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pSwapChain);

	if (m_pDepthStencilView != nullptr)
	{
		m_pDepthStencilView->Release();
		delete m_pDepthStencilView;
		m_pDepthStencilView = nullptr;
	}

	if (m_pD3dImmediateContext)
		m_pD3dImmediateContext->ClearState();

	ReleaseCOM(m_pD3dImmediateContext);
	ReleaseCOM(m_pD3dDevice);


	ReleaseCOM(m_pWireframeRS);
	ReleaseCOM(m_pSolidRS);
	ReleaseCOM(m_pNoCullModeRS);
	ReleaseCOM(m_NormalDSS);
	ReleaseCOM(m_Normal_StencilDSS);
	ReleaseCOM(m_Normal_Equal_StencilDSS);
	ReleaseCOM(m_ShadowDSS);
	ReleaseCOM(m_OffDepthCheckDSS);
	ReleaseCOM(m_MRTDSS);

	if (m_EngineCamera != nullptr)
	{
		delete m_EngineCamera;
		m_EngineCamera = nullptr;
	}

	if (m_pFrustum != nullptr)
	{
		delete m_pFrustum;
		m_pFrustum = nullptr;
	}

	if (m_pRenderQueue != nullptr)
	{
		delete m_pRenderQueue;
		m_pRenderQueue = nullptr;
	}

	if (m_pSkyBox != nullptr)
	{
		delete m_pSkyBox;
		m_pSkyBox = nullptr;
	}

	for (auto& rtv : m_RTVs)
	{
		if (rtv != nullptr)
		{
			rtv->Release();
			delete rtv;
			rtv = nullptr;
		}
	}
	m_RTVs.clear();

	if (m_pPostProcess != nullptr)
	{
		delete m_pPostProcess;
		m_pPostProcess = nullptr;
	}

	if (m_pSSAO != nullptr)
	{
		delete m_pSSAO;
		m_pSSAO = nullptr;
	}

	if (m_pUIRenderer != nullptr)
	{
		delete m_pUIRenderer;
		m_pUIRenderer = nullptr;
	}

	if (m_pLightManager != nullptr)
	{
		delete m_pLightManager;
		m_pLightManager = nullptr;
	}

	if (m_pProjecedTexturePass != nullptr)
	{
		delete m_pProjecedTexturePass;
		m_pProjecedTexturePass = nullptr;
	}

	if (m_ResourceManager != nullptr)
	{
		delete m_ResourceManager;
		m_ResourceManager = nullptr;
	}

	ConstBufferManager::Finalize();
	SamplerState::Finalize();
	Renderer::Release();
	RenderTargetView::Finialize();
#ifdef _DEBUG
	HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
	decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));

	IDXGIDebug* debug;
	GetDebugInterface(IID_PPV_ARGS(&debug));

	OutputDebugStringW(L"-------Com Object Memory Leak-------\n");

	debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
	OutputDebugStringW(L"-------Com Object Memory Leak-------\n");

	debug->Release();
#endif // _DEBUG

	//delete m_graphicInstance;
}

void TLGraphicsEngine::GraphicsEngine::InitializeGraphic(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight)
{
	HRESULT _hResult = CoInitialize(NULL);

	m_HInstance = hinst;
	m_Hwnd = hWnd;
	m_ClientWidth = screenWidth;
	m_ClientHeight = screenHeight;

	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;

#if defined(DEBUG) | defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&m_pD3dDevice,
		&featureLevel,
		&m_pD3dImmediateContext
	);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		//return S_FALSE;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		//return S_FALSE;
	}

	HRESULT _hr = m_pD3dImmediateContext->QueryInterface(__uuidof(m_User), &m_User);

	HR(_hr, "failed to get d3d11-user-defined-annotation");

	HR(m_pD3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
	assert(m_4xMsaaQuality > 0);

	DXGI_SWAP_CHAIN_DESC sd; // BufferDesc 후면 버퍼의 속성 구조체
	sd.BufferDesc.Width = m_ClientWidth; // 버퍼의 너비
	sd.BufferDesc.Height = m_ClientHeight; // 버퍼의 높이
	sd.BufferDesc.RefreshRate.Numerator = 60; // 갱신률 분자
	sd.BufferDesc.RefreshRate.Denominator = 1; // 갱신률 분모
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 후면 버퍼 픽셀 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 디스플레이 스캔라인 모드
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 디스플레이 비례 모드

	if (m_bEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼의 용도를 서술하는 구조체, 후면 버퍼에 렌더링을 한다고 지정했음
	sd.BufferCount = 1; // 교환 사슬에서 사용할 후면 버퍼의 개수, 일반적으로 한개만 사용
	sd.OutputWindow = m_Hwnd; // 렌더링 결과를 표시 할 창의 핸들값
	sd.Windowed = true; // 창모드 true, 전체화면 모드 flase
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 교환 효과를 서술하는 구조체, 지금은 디스플레이 구동기가 가장 효율적인 제시 방법을 선택함
	sd.Flags = 0; // 추가적인 플래그

	IDXGIDevice* dxgiDevice = nullptr;
	HR(m_pD3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = nullptr;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	if (dxgiFactory != nullptr)
	{
		HR(dxgiFactory->CreateSwapChain(m_pD3dDevice, &sd, &m_pSwapChain));
	}

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	m_EngineCamera = new EngineCamera();
	m_pUIRenderer = new UIRenderer();
	if (m_graphicInstance == nullptr) m_graphicInstance = this;

	m_pDepthStencilView = new DepthStencilView();

	m_ResourceManager = GraphicResourceManager::Instance();

	for (int i = 0; i < 8; i++)
	{
		m_RTVs.push_back(new RenderTargetView);
	}

	RenderTargetView::Init();

	m_pRenderer = Renderer::Instance();
	m_pRenderer->Initialize(m_EngineCamera,
		m_ResourceManager->GetShaderRef("Deferred_VS_Mesh"),
		m_ResourceManager->GetShaderRef("Deferred_VS_Skin"),
		m_ResourceManager->GetShaderRef("Quard_VS"),
		m_ResourceManager->GetShaderRef("Texture_PS"),
		m_ResourceManager->GetShaderRef("Gizmo_VS"),
		m_ResourceManager->GetShaderRef("Gizmo_PS")
		);

	CreateRenderStates();

	// 폰트 생성
	m_pUIRenderer->Create(m_pD3dDevice, m_pSolidRS, m_NormalDSS);

	m_pRenderQueue = new RenderQueue;
	m_pFrustum = new Frustum(m_EngineCamera);
	//m_SkyBox = new SkyBox(m_EngineCamera, m_ResourceManager->GetMeshRef("Cube"), m_ResourceManager->GetMaterialRef("SkyBox"));
	m_pSkyBox = new SkyBox(m_EngineCamera);
	m_pSkyBox->Initialize();
	m_pPostProcess = new PostProcess;
	m_pPostProcess->Initialize();

	m_pSSAO = new SSAO;
	m_pSSAO->Initialize();

	m_pLightManager = new LightManager();
	m_pLightManager->Initialize();

	m_pProjecedTexturePass = new ProjecedTexturePass();

	OnResize();

	GetAdapterInfo();
}

void TLGraphicsEngine::GraphicsEngine::OnResize()
{
	assert(m_pD3dImmediateContext);
	assert(m_pD3dDevice);
	assert(m_pSwapChain);

	ReleaseCOM(m_pRenderTargetView);

	HR(m_pSwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	ID3D11Texture2D* backBuffer = nullptr;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));

	if (backBuffer != nullptr)
	{
		HR(m_pD3dDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView));
	}

	ReleaseCOM(backBuffer);

	m_RTVs[0]->OnResize(m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R16G16B16A16_UNORM);
	m_RTVs[1]->OnResize(m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_RTVs[2]->OnResize(m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_RTVs[3]->OnResize(m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_RTVs[4]->OnResize(m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_RTVs[5]->OnResize(m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_RTVs[6]->OnResize(m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM);
	m_RTVs[7]->OnResize(m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM);

	m_pDepthStencilView->OnResize(m_ClientWidth, m_ClientHeight, true);

	if (m_pSSAO != nullptr)
	{
		m_pSSAO->OnResize(m_ClientWidth, m_ClientHeight);
	}

	if (m_pPostProcess != nullptr)
	{
		m_pPostProcess->OnResize(m_ClientWidth, m_ClientHeight);
	}

	if (m_pProjecedTexturePass != nullptr)
	{
		m_pProjecedTexturePass->OnResize(m_ClientWidth, m_ClientHeight);
	}

	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
	m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
	m_ScreenViewport.MinDepth = 0.f;
	m_ScreenViewport.MaxDepth = 1.f;

	m_pD3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);

	m_EngineCamera->OnResize(GetAspectRatio());
}

void TLGraphicsEngine::GraphicsEngine::SetResize(UINT width, UINT height, WPARAM wParam)
{
	m_ClientWidth = width;
	m_ClientHeight = height;
	if (m_pD3dDevice)
	{
		if (wParam == SIZE_MINIMIZED)
		{
			m_isGraphicEnginePause = true;
			m_isMinimized = true;
			m_isMaximized = false;
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			m_isGraphicEnginePause = false;
			m_isMinimized = false;
			m_isMaximized = true;
			OnResize();
		}
		else if (wParam == SIZE_RESTORED)
		{

			// Restoring from minimized state?
			if (m_isMinimized)
			{
				m_isGraphicEnginePause = false;
				m_isMinimized = false;
				OnResize();
			}

			// Restoring from maximized state?
			else if (m_isMaximized)
			{
				m_isGraphicEnginePause = false;
				m_isMaximized = false;
				OnResize();
			}
			else if (m_isResizing)
			{
			}
			else
			{
				OnResize();
			}
		}
	}
}

void TLGraphicsEngine::GraphicsEngine::RegistRenderQueue(IMesh* mesh, vector<IMaterial*> mats, float WM[16], int renderType, float* m_BoneTMList /*= nullptr*/, int size /*= 0*/)
{
	Mesh* _mesh = reinterpret_cast<Mesh*>(mesh);

#ifdef _DEBUG
	if (_mesh == nullptr)
	{
		// _mesh가 없음
		assert(false);
		return;
	}
#endif

	std::vector<Material*> _mats;
	for (auto material : mats)
	{
		_mats.push_back(reinterpret_cast<Material*>(material));
	}

	switch (renderType)
	{
		case (int)RenderObject::RenderType::Mesh:
		{
			m_pRenderQueue->RegistMesh(_mesh, _mats, WM);
			break;
		}
		case (int)RenderObject::RenderType::SkinnedMesh:
		{
			m_pRenderQueue->RegistSkinnedMesh(_mesh, _mats, WM, m_BoneTMList, size);
			break;
		}
		case (int)RenderObject::RenderType::ProjectedMesh:
		{
			m_pRenderQueue->RegistProjectedMesh(_mesh, _mats, WM);
			m_needRecoredDepth = true;
			break;
		}
		case (int)RenderObject::RenderType::OutLine:
		{
			m_pRenderQueue->RegistOutLine(_mesh, WM);
			break;
		}
	}
}

void TLGraphicsEngine::GraphicsEngine::RegistTextUIRenderQueue(float x, float y, const char* text, float* color, int fontSize, float lineSpacing, float alignX, float alignY)
{
	DirectX::XMFLOAT4 color4{ *color, *(color + 1), *(color + 2) , *(color + 3) };

	m_pUIRenderer->RegistTextUI({ x, y }, text, color4, fontSize, lineSpacing, { alignX, alignY });
}

void TLGraphicsEngine::GraphicsEngine::RegistImageUIRenderQueue(float x, float y, ITexture* texture, float* color, RECT rect, int imageType, float value)
{
	DirectX::XMFLOAT4 color4{ *color, *(color + 1), *(color + 2) , *(color + 3) };

	Texture* _texture = reinterpret_cast<Texture*>(texture);

	m_pUIRenderer->RegistImageUI({ x, y }, _texture, color4, rect, imageType, value);
}

void TLGraphicsEngine::GraphicsEngine::RegistGizmo(int gizmoType, float WM[16], float* color)
{
	if(!m_gizmo)
		return;
	DirectX::XMFLOAT4 _color{ *color, *(color + 1), *(color + 2) , *(color + 3) };

	Mesh* _mesh = m_ResourceManager->GetDebugMesh(gizmoType);

	if (_mesh == nullptr)
	{
		// 기즈모 정보가 없는 매쉬입니다.
		assert(false);
	}

	m_pRenderQueue->RegistGizmo(_mesh, WM, _color);
}

void TLGraphicsEngine::GraphicsEngine::RegistRenderQueue(UID meshUID, vector<UID> matUIDs, float WM[16], int renderType, float* m_BoneTMList /*= nullptr*/, int size /*= 0*/)
{
	Mesh* _mesh = m_ResourceManager->GetMesh(meshUID);

#ifdef _DEBUG
	if (_mesh == nullptr)
	{
		// _mesh가 없음
		assert(false);
		return;
	}
#endif

	std::vector<Material*> _mats;
	for (auto uid : matUIDs)
	{
		_mats.push_back(m_ResourceManager->GetMaterial(uid));
	}

	switch (renderType)
	{
	case (int)RenderObject::RenderType::Mesh:
	{
		m_pRenderQueue->RegistMesh(_mesh, _mats, WM);
		break;
	}
	case (int)RenderObject::RenderType::SkinnedMesh:
	{
		m_pRenderQueue->RegistSkinnedMesh(_mesh, _mats, WM, m_BoneTMList, size);
		break;
	}
	case (int)RenderObject::RenderType::ProjectedMesh:
	{
		m_pRenderQueue->RegistProjectedMesh(_mesh, _mats, WM);
		break;
	}
	}
}

void TLGraphicsEngine::GraphicsEngine::SetCameraPosition(float posX, float posY, float posZ)
{
	m_EngineCamera->SetPosition(posX, posY, posZ);
}

void TLGraphicsEngine::GraphicsEngine::SetCameraLookAt(float lookX, float lookY, float lookZ)
{
	m_EngineCamera->SetLookAt(lookX, lookY, lookZ);
}

void TLGraphicsEngine::GraphicsEngine::SetCameraUp(float upX, float upY, float upZ)
{
	m_EngineCamera->SetUp(upX, upY, upZ);
}

void TLGraphicsEngine::GraphicsEngine::SetCameraRight(float rightX, float rightY, float rightZ)
{
	m_EngineCamera->SetRight(rightX, rightY, rightZ);
}

void TLGraphicsEngine::GraphicsEngine::SetLookAt(float posX, float posY, float posZ, float targetX, float targetY, float targetZ, float worldUpX, float worldUpY, float worldUpZ)
{
	DirectX::XMVECTOR camPos = { posX, posY, posZ, 0.0f };
	DirectX::XMVECTOR targetPos = { targetX, targetY, targetZ, 0.0f };
	DirectX::XMVECTOR upDir = { worldUpX, worldUpY, worldUpZ, 0.0f };

	m_EngineCamera->LookAt(camPos, targetPos, upDir);
}

void TLGraphicsEngine::GraphicsEngine::SetLens(float fovY, float zn, float zf)
{
	m_EngineCamera->SetLens(fovY, GetAspectRatio(), zn, zf);
}

void TLGraphicsEngine::GraphicsEngine::FrustumUpdate(float posx, float posy, float posz, float forwardx, float forwardy, float forwardz, float upx, float upy, float upz, float fovY, float aspectRatio, float nearZ, float farZ)
{
	m_pFrustum->FrustumUpdate(posx, posy, posz, forwardx, forwardy, forwardz, upx, upy, upz, fovY, GetAspectRatio(), nearZ, farZ);
}

TLGraphicsEngine::IMesh* TLGraphicsEngine::GraphicsEngine::GetIMesh(const char* file)
{
	return m_ResourceManager->GetMesh(file);
}

TLGraphicsEngine::ITexture* TLGraphicsEngine::GraphicsEngine::GetITexture(const char* file)
{
	return m_ResourceManager->GetTexture(file);
}

TLGraphicsEngine::IMaterial* TLGraphicsEngine::GraphicsEngine::GetIMaterial(const char* file)
{
	return m_ResourceManager->GetMaterial(file);
}

TLGraphicsEngine::Shader* TLGraphicsEngine::GraphicsEngine::GetShader(const char* file)
{
	return m_ResourceManager->GetShader(file);
}

TLGraphicsEngine::Shader* TLGraphicsEngine::GraphicsEngine::GetShader(UID uid)
{
	return m_ResourceManager->GetShader(uid);
}

TLGraphicsEngine::IMesh* TLGraphicsEngine::GraphicsEngine::GetIMesh(UID uid)
{
	return m_ResourceManager->GetMesh(uid);
}

TLGraphicsEngine::ITexture* TLGraphicsEngine::GraphicsEngine::GetITexture(UID uid)
{
	return m_ResourceManager->GetTexture(uid);
}

TLGraphicsEngine::IMaterial* TLGraphicsEngine::GraphicsEngine::GetIMaterial(UID uid)
{
	return m_ResourceManager->GetMaterial(uid);
}

shared_ptr<TLGraphicsEngine::IMesh> TLGraphicsEngine::GraphicsEngine::GetIMeshRef(const char* file)
{
	return m_ResourceManager->GetMeshRef(file);
}

shared_ptr<TLGraphicsEngine::ITexture> TLGraphicsEngine::GraphicsEngine::GetITextureRef(const char* file)
{
	return m_ResourceManager->GetTextureRef(file);
}

shared_ptr<TLGraphicsEngine::IMaterial> TLGraphicsEngine::GraphicsEngine::GetIMaterialRef(const char* file)
{
	return m_ResourceManager->GetMaterialRef(file);
}

std::shared_ptr<TLGraphicsEngine::Shader> TLGraphicsEngine::GraphicsEngine::GetShaderRef(const char* file)
{
	return m_ResourceManager->GetShaderRef(file);
}

std::shared_ptr<TLGraphicsEngine::IMesh> TLGraphicsEngine::GraphicsEngine::CreateMesh(const char* name, UID uid)
{
	return m_ResourceManager->CreateMesh(name, uid);
}

std::shared_ptr<TLGraphicsEngine::IMaterial> TLGraphicsEngine::GraphicsEngine::CreateMaterial(const char* name, UID uid)
{
	return m_ResourceManager->CreateMaterial(name, uid);
}

std::shared_ptr<TLGraphicsEngine::ITexture> TLGraphicsEngine::GraphicsEngine::CreateTexture(const char* name, UID uid)
{
	return m_ResourceManager->CreateTexture(name, uid);
}

std::shared_ptr<TLGraphicsEngine::Shader> TLGraphicsEngine::GraphicsEngine::GetShaderRef(UID uid)
{
	return m_ResourceManager->GetShaderRef(uid);
}

shared_ptr<TLGraphicsEngine::IMesh> TLGraphicsEngine::GraphicsEngine::GetIMeshRef(UID uid)
{
	return m_ResourceManager->GetMeshRef(uid);
}

shared_ptr<TLGraphicsEngine::ITexture> TLGraphicsEngine::GraphicsEngine::GetITextureRef(UID uid)
{
	return m_ResourceManager->GetTextureRef(uid);
}

shared_ptr<TLGraphicsEngine::IMaterial> TLGraphicsEngine::GraphicsEngine::GetIMaterialRef(UID uid)
{
	return m_ResourceManager->GetMaterialRef(uid);
}

std::shared_ptr<TLGraphicsEngine::Shader> TLGraphicsEngine::GraphicsEngine::CreateShader(const char* name, UID uid)
{
	return m_ResourceManager->CreateShader(name, uid);
}

void TLGraphicsEngine::GraphicsEngine::LoadPrefab(IPrefabInfo& prefab, const char* file)
{
	switch (prefab.m_PrefabType)
	{
		case IPrefabInfo::ePrefabType::ASE:
		{
			m_ResourceManager->LoadASE(file/*, prefab*/);
			break;
		}
		case IPrefabInfo::ePrefabType::FBX:
		{
			break;
		}
		case IPrefabInfo::ePrefabType::UnKnown:
		default:
#ifdef _DEBUG
			assert(false);
#endif // _DEBUG
			break;
	}
}

void TLGraphicsEngine::GraphicsEngine::OnMouseMove(int screenX, int screenY)
{
	m_NDCMousePosX = (+2.0f * screenX / m_ClientWidth - 1.0f);
	m_NDCMousePosY = (-2.0f * screenY / m_ClientHeight + 1.0f);
}

void TLGraphicsEngine::GraphicsEngine::SetSSAO(bool value)
{
	m_bOnOffSSAO = value;
}

void TLGraphicsEngine::GraphicsEngine::SetBlur(bool value)
{
	m_pPostProcess->OnOffBlur(value);
}

void TLGraphicsEngine::GraphicsEngine::SetBloom(bool value)
{
	m_pPostProcess->OnOffBloom(value);
}

void TLGraphicsEngine::GraphicsEngine::SetBloomThreshold(float min, float max)
{
	m_pPostProcess->SetBloomThreshold(min, max);
}

void TLGraphicsEngine::GraphicsEngine::SetToneMap(bool value)
{
	m_pPostProcess->OnOffToneMapping(value);
}

void TLGraphicsEngine::GraphicsEngine::SetToneMapExposure(float value)
{
	m_pPostProcess->SetToneMapExposure(value);
}

void TLGraphicsEngine::GraphicsEngine::SetIBLFactor(float value)
{
	m_fIBLFactor = value;
}

int TLGraphicsEngine::GraphicsEngine::AddLight()
{
	return m_pLightManager->AddLight();
}

void TLGraphicsEngine::GraphicsEngine::DeleteLight(int id)
{
	m_pLightManager->DeleteLight(id);
}

void TLGraphicsEngine::GraphicsEngine::SetLightEnable(int id, bool enable)
{
	m_pLightManager->SetLightEnable(id, enable);
}

void TLGraphicsEngine::GraphicsEngine::UpdateLight(int id, int type, float range, float fallOff, float spotAngle, float fallOffAngle, float power, float* pos, float* dir, float* color)
{
	XMFLOAT3 _color = { *color, *(color + 1), *(color + 2) };
	XMFLOAT3 _position = { *pos, *(pos + 1), *(pos + 2) };
	XMFLOAT3 _direction = { *dir, *(dir + 1), *(dir + 2) };

	m_pLightManager->UpdateLight(id, type, range, fallOff, spotAngle, fallOffAngle, power, _position, _direction, _color);
}

void TLGraphicsEngine::GraphicsEngine::ClearLight()
{
	m_pLightManager->ClearLight();
	m_pProjecedTexturePass->Reset();
}

void TLGraphicsEngine::GraphicsEngine::OnOffDebug()
{
	m_OnOffDebug = !m_OnOffDebug;
}

void TLGraphicsEngine::GraphicsEngine::OnDebug(int type)
{
	switch (type)
	{
		case 0:
		{
			m_gizmo = !m_gizmo;
			break;
		}
		case 1:
		{
			m_mrt = !m_mrt;
			break;
		}
		case 2:
		{
			m_postProcess = !m_postProcess;
			break;
		}
		default:
			break;
	}
}

void TLGraphicsEngine::GraphicsEngine::CreateMeshBuffer(shared_ptr<Mesh> mesh)
{
	mesh->CreateMeshBuffer(m_pD3dDevice);
}

void TLGraphicsEngine::GraphicsEngine::CheckPicking(Mesh* mesh, XMMATRIX meshWorld)
{
	// Todo : Picking
}

void TLGraphicsEngine::GraphicsEngine::UpdateFrameConstBuffer()
{
	auto _constBuffer = ConstBufferManager::Instance()->GetConstBuffer("cbPerFrame");

	PerFrame_CB frameCB;

	XMMATRIX world = DirectX::XMMatrixTranslation(m_EngineCamera->GetPosition().x,
		m_EngineCamera->GetPosition().y, m_EngineCamera->GetPosition().z);
	XMMATRIX projInv = XMMatrixInverse(nullptr, m_EngineCamera->GetProjTM());

	static const XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	XMMATRIX viewToTexSpace = DirectX::XMMatrixMultiply(m_EngineCamera->GetProjTM(), T);
	frameCB.cameraInfo.cameraPosision = m_EngineCamera->GetPosition();

	DirectX::XMStoreFloat4x4(&frameCB.cameraInfo.camWorld, world);
	DirectX::XMStoreFloat4x4(&frameCB.cameraInfo.camWorldViewProj, world * m_EngineCamera->GetViewProjTM());
	DirectX::XMStoreFloat4x4(&frameCB.cameraInfo.view, m_EngineCamera->GetViewTM());
	DirectX::XMStoreFloat4x4(&frameCB.cameraInfo.proj, m_EngineCamera->GetProjTM());
	DirectX::XMStoreFloat4x4(&frameCB.cameraInfo.projInv, projInv);
	DirectX::XMStoreFloat4x4(&frameCB.cameraInfo.viewProj, m_EngineCamera->GetViewProjTM());
	DirectX::XMStoreFloat4x4(&frameCB.cameraInfo.viewToTexSpace, viewToTexSpace);


	if (m_bOnOffSSAO && m_pSSAO != nullptr)
	{

		memcpy(&frameCB.ssaoInfo.offSetVectors[0], &m_pSSAO->m_Offsets[0], sizeof(XMFLOAT4) * 14);
		memcpy(&frameCB.ssaoInfo.frustumCorners[0], &m_pSSAO->m_FrustumFarCorners[0], sizeof(XMFLOAT4) * 4);

		frameCB.ssaoInfo.occlusionRadius = 0.5f;
		frameCB.ssaoInfo.occlusionFadeStart = 0.2f;
		frameCB.ssaoInfo.occlusionFadeEnd = 2.0f;
		frameCB.ssaoInfo.surfaceEpsilon = 0.05f;
	}

	m_pD3dImmediateContext->UpdateSubresource(_constBuffer->GetBuffer(), 0, nullptr, &frameCB, 0, 0);

	m_pD3dImmediateContext->VSSetConstantBuffers(0, 1, _constBuffer->GetBufferRef());
	m_pD3dImmediateContext->PSSetConstantBuffers(0, 1, _constBuffer->GetBufferRef());
}

void TLGraphicsEngine::GraphicsEngine::GetLightBuffer(Light_CB* lightcb, std::vector<ID3D11ShaderResourceView**>& srvs)
{
	srvs.push_back(m_pSkyBox->m_BRDFTextrue->GetSRVR());
	srvs.push_back(m_pSkyBox->m_IrradianceTextrue->GetSRVR());
	srvs.push_back(m_pSkyBox->m_SkyBoxTextrue->GetSRVR());

	lightcb->IBLFactor = m_fIBLFactor;
	m_pLightManager->UpdateLightBuffer(lightcb, srvs);
}

void TLGraphicsEngine::GraphicsEngine::BeginEvent(const wchar_t* event)
{
	m_User->BeginEvent(event);
}

void TLGraphicsEngine::GraphicsEngine::EndEvent()
{
	m_User->EndEvent();
}

void TLGraphicsEngine::GraphicsEngine::BeginRender()
{
	assert(m_pD3dImmediateContext);
	assert(m_pSwapChain);

	m_EngineCamera->UpdateView();

	m_pD3dImmediateContext->OMSetDepthStencilState(m_Normal_StencilDSS, 255);

	// DXTKFont에서 블렌드 스테이트를 건들이고 있었다. 기본값으로 돌려준다.
	FLOAT blendFactor[4] = {};
	m_pD3dImmediateContext->OMSetBlendState(nullptr, blendFactor, 0xffffffff);

	m_pD3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);

	if (m_bOnOffSSAO && m_pSSAO != nullptr)
	{
		m_pSSAO->Setting(m_EngineCamera->GetFovY(), m_EngineCamera->GetFar(), m_EngineCamera->GetAspect());
	}

	UpdateFrameConstBuffer();
#ifdef _DEBUG
	m_iDrawObjCnt = 0;
#endif // _DEBUG
}

void TLGraphicsEngine::GraphicsEngine::Render()
{
	XMFLOAT3 boundingVertex[8];
	m_PickingCnt = 0;
	while (!m_pRenderQueue->m_BegineRenderQueue.empty())
	{
		RenderObject renderObj = m_pRenderQueue->m_BegineRenderQueue.front();
		m_pRenderQueue->m_BegineRenderQueue.pop();

		XMMATRIX world = DirectX::XMLoadFloat4x4(&renderObj.m_World);
		m_pFrustum->Setting(boundingVertex, renderObj.m_Mesh, world);
		if (m_pFrustum->CheckBoundingBox(boundingVertex))
		{
			m_pRenderQueue->m_DrawRenderQueue.push(std::move(renderObj));

			CheckPicking(renderObj.m_Mesh, world);
			//CheckPicking(boundingVertex, world);
		}
	}


	RenderTargetView::Sets(m_RTVs.data() + 1, 5, m_pDepthStencilView);

	std::vector<RenderObject> renderObjs;
	m_iDrawObjCnt = (int)m_pRenderQueue->m_DrawRenderQueue.size();

	std::queue<RenderObject> m_ProjectedObjects;
	std::queue<RenderObject> m_OutLineObjects;

	BeginEvent(TEXT("Mesh Pass"));

	while (!m_pRenderQueue->m_DrawRenderQueue.empty())
	{
		RenderObject renderObj = m_pRenderQueue->m_DrawRenderQueue.front();
		m_pRenderQueue->m_DrawRenderQueue.pop();

		renderObjs.push_back(renderObj);
		XMMATRIX world = DirectX::XMLoadFloat4x4(&renderObj.m_World);

		switch (renderObj.m_RenderType)
		{
			case RenderObject::RenderType::Mesh:
			{
				m_pRenderer->RenderMesh(m_pD3dImmediateContext, &renderObj);
				break;
			}
			case RenderObject::RenderType::SkinnedMesh:
			{
				m_pRenderer->RenderMesh(m_pD3dImmediateContext, &renderObj);
				break;
			}
			case RenderObject::RenderType::ProjectedMesh:
			{
				m_ProjectedObjects.push(std::move(renderObj));
				break;
			}
			case RenderObject::RenderType::OutLine:
			{
				m_OutLineObjects.push(std::move(renderObj));
			}
		}
	}

	EndEvent();

	BeginEvent(TEXT("Light Pass"));

	if (m_pLightManager)
	{
		m_pLightManager->CalcLight(renderObjs);
	}

	EndEvent();


	BeginEvent(TEXT("Project Pass"));

	while (!m_ProjectedObjects.empty())
	{
		RenderObject renderObj = m_ProjectedObjects.front();
		m_ProjectedObjects.pop();

		XMMATRIX world = DirectX::XMLoadFloat4x4(&renderObj.m_World);

		m_pProjecedTexturePass->MakeProjectedTexture(&renderObj);
	}

	EndEvent();

	BeginEvent(TEXT("Outline Pass"));

	{
		m_pD3dImmediateContext->OMSetRenderTargets(1, m_RTVs[6]->GetRTVR(), m_pPostProcess->GetOutLineDSV()->GetDSV());
		m_pD3dImmediateContext->ClearDepthStencilView(m_pPostProcess->GetOutLineDSV()->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

		D3D11_VIEWPORT vs;
		vs.Height = m_ClientHeight;
		vs.Width = m_ClientWidth;
		vs.TopLeftX = 0;
		vs.TopLeftY = 0;
		vs.MaxDepth = 1;
		vs.MinDepth = 0;

		m_pD3dImmediateContext->RSSetViewports(1, &vs);

		while (!m_OutLineObjects.empty())
		{
			RenderObject renderObj = m_OutLineObjects.front();
			m_OutLineObjects.pop();

			XMMATRIX world = DirectX::XMLoadFloat4x4(&renderObj.m_World);

			m_pRenderer->RenderOutLine(m_pD3dImmediateContext, &renderObj);
		}

		m_pPostProcess->OutLine(nullptr);
	}

	EndEvent();


	if (m_bOnOffSSAO && m_pSSAO != nullptr)
	{
		m_pSSAO->OnSSAO(m_RTVs[2]->GetSRV(), m_RTVs[5]->GetSRV());
	}

	MRTRender();
}

void TLGraphicsEngine::GraphicsEngine::EndRender()
{
	m_pD3dImmediateContext->OMSetDepthStencilState(m_NormalDSS, 0);

	float WM[16] =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	XMFLOAT4 _color = { 1, 1, 1, 1 };
	
	if (m_gizmo)
	{
		m_pRenderQueue->RegistGizmo(m_ResourceManager->GetDebugMesh(2), WM, _color);
		m_pRenderQueue->RegistGizmo(m_ResourceManager->GetDebugMesh(1), WM, _color);
	}

	BeginEvent(TEXT("Gizmo Pass"));

	while (!m_pRenderQueue->m_DrawGizmoQueue.empty())
	{
		GizmoObject renderObj = m_pRenderQueue->m_DrawGizmoQueue.front();
		m_pRenderQueue->m_DrawGizmoQueue.pop();

		m_pRenderer->RenderGizmo(m_pD3dImmediateContext, &renderObj);
	}

	EndEvent();

	DirectX::XMFLOAT4 test3(1.f, 1.f, 0.f, 1.f);
	DirectX::XMFLOAT4 _white(1.f, 1.f, 1.f, 1.f);

	BeginEvent(TEXT("UI Pass"));
	
	m_pUIRenderer->BeginRender();
	m_pUIRenderer->OnRender();
	m_pUIRenderer->EndRender();

	EndEvent();

	HR(m_pSwapChain->Present(0, 0));
}

void TLGraphicsEngine::GraphicsEngine::DrawText(int x, int y, float r, float g, float b, float a, TCHAR* pch, ...)
{
	TCHAR _buffer[1024] = L"";
	va_list vl;
	va_start(vl, pch);
	_vstprintf(_buffer, 1024, pch, vl);
	va_end(vl);

	DirectX::XMFLOAT4 color = { r, g, b, a };

	m_pUIRenderer->DrawTextColor(x, y, color, _buffer);
}

void TLGraphicsEngine::GraphicsEngine::MRTRender()
{
	const float color[] = { 0, 0, 0, 0 };

	m_pD3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);

	m_pD3dImmediateContext->ClearRenderTargetView(m_RTVs[0]->GetRTV(), &color[0]);

	m_pD3dImmediateContext->OMSetRenderTargets(1, m_RTVs[0]->GetRTVR(), nullptr);

	if (m_pSkyBox != nullptr)
	{
		BeginEvent(TEXT("Sky Box Pass"));

		m_pSkyBox->Render();

		EndEvent();
	}

	{
		BeginEvent(TEXT("Deferred Merge Pass"));

		m_pD3dImmediateContext->OMSetDepthStencilState(m_MRTDSS, 128);
		// 렌더스테이트
		m_pD3dImmediateContext->RSSetState(m_pSolidRS);

		// 입력 배치 객체 셋팅
		m_pD3dImmediateContext->IASetInputLayout(RenderTargetView::m_DeferredVS->GetInputLayout());
		m_pD3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = sizeof(QuadVertex);
		UINT offset = 0;

		m_pD3dImmediateContext->IASetVertexBuffers(0, 1, &RenderTargetView::m_VB, &stride, &offset);
		m_pD3dImmediateContext->IASetIndexBuffer(RenderTargetView::m_IB, DXGI_FORMAT_R32_UINT, 0);

		m_pD3dImmediateContext->VSSetShader(RenderTargetView::m_DeferredVS->GetShader<ID3D11VertexShader>(), nullptr, 0);

		std::vector<ID3D11ShaderResourceView**> srvs =
		{
			m_RTVs[1]->GetSRVR(),
			m_RTVs[2]->GetSRVR(),
			//m_RTVs[3]->GetSRVR(),
			m_pDepthStencilView->GetSRVR(),
			m_RTVs[4]->GetSRVR(),
			m_RTVs[5]->GetSRVR(),
			m_pSkyBox->m_BRDFTextrue->GetSRVR(),
			m_pSkyBox->m_IrradianceTextrue->GetSRVR(),
			m_pSkyBox->m_SkyBoxTextrue->GetSRVR()
		};

		std::vector<ID3D11ShaderResourceView*> nullSRV;

		Light_CB cb;
		cb.IBLFactor = m_fIBLFactor;
		m_pLightManager->UpdateLightBuffer(&cb, srvs);

		std::vector<CBBase*> cbs =
		{
			&cb
		};

		if (m_bOnOffSSAO)
		{
			m_pSSAO->GetSSAOMap();
			srvs[5] = m_RTVs[6]->GetSRVR();
			RenderTargetView::m_DeferredSSAOPS->Update(cbs.data(), srvs);
			m_pD3dImmediateContext->PSSetShader(RenderTargetView::m_DeferredSSAOPS->GetShader<ID3D11PixelShader>(), nullptr, 0);
		}
		else
		{
			RenderTargetView::m_DeferredPS->Update(cbs.data(), srvs);
			m_pD3dImmediateContext->PSSetShader(RenderTargetView::m_DeferredPS->GetShader<ID3D11PixelShader>(), nullptr, 0);
		}

		m_pD3dImmediateContext->DrawIndexed(6, 0, 0);

		EndEvent();

		nullSRV.resize(srvs.size(), nullptr);

		if (m_bOnOffSSAO)
		{
			m_pD3dImmediateContext->PSSetShaderResources(0, static_cast<UINT>(srvs.size()), nullSRV.data());
		}
		else
		{
			m_pD3dImmediateContext->PSSetShaderResources(0, static_cast<UINT>(srvs.size()), nullSRV.data());
		}
	}

	ID3D11RenderTargetView* rtvNull[5] = { nullptr, };

	m_pD3dImmediateContext->OMSetRenderTargets(5, &rtvNull[0], nullptr);

	if (m_pProjecedTexturePass != nullptr && m_needRecoredDepth)
	{
		BeginEvent(TEXT("Projector Pass"));

		//m_pProjecedTexturePass->RecordDepth(m_RTVs[3]);
		m_pProjecedTexturePass->RecordDepth(m_pDepthStencilView);
		m_pProjecedTexturePass->SetPass(m_RTVs[4], m_RTVs[0]);

		EndEvent();
	}

	BeginEvent(TEXT("Post Process Pass"));
	// PostProcess Pass
	RenderTargetView* setEffectRTV = m_pPostProcess->PostProcessPass(m_RTVs[0], m_RTVs[7]);

	BeginEvent(TEXT("Outline Add"));

	//m_pPostProcess->AddOutLine(m_RTVs[7], m_RTVs[0]);
	//m_pPostProcess->AddOutLine(setEffectRTV, m_RTVs[7]);

	EndEvent();

	EndEvent();

	//RenderToMain(m_RTVs[0]);
	RenderToMain(setEffectRTV);
}

void TLGraphicsEngine::GraphicsEngine::RenderToMain(RenderTargetView* rtv)
{
	const float color[4] = {};

	BeginEvent(TEXT("Final Pass"));

	m_pD3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView, &color[0]);

	m_pD3dImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView->GetDSV());

	m_pD3dImmediateContext->OMSetDepthStencilState(m_OffDepthCheckDSS, 0);

	m_pRenderer->Render2D(m_pD3dImmediateContext, rtv->GetSRV());

	EndEvent();


	if (m_OnOffDebug)
	{
		std::vector<ID3D11ShaderResourceView*> _debugInfo;

		if (m_mrt)
		{
			_debugInfo.push_back(m_RTVs[1]->GetSRV());
			_debugInfo.push_back(m_RTVs[2]->GetSRV());
			_debugInfo.push_back(m_RTVs[3]->GetSRV());
			_debugInfo.push_back(m_RTVs[4]->GetSRV());
			_debugInfo.push_back(m_RTVs[5]->GetSRV());
		}

		if (m_postProcess)
		{
			_debugInfo.push_back(m_pSSAO->GetSSAOMap());
			_debugInfo.push_back(m_pPostProcess->GetOutLineRTV()->GetSRV());
		}

		int x = 0;
		int y = 0;

		float topX, topY, width, height;
		width = m_ClientWidth * 0.2f;
		height = m_ClientHeight * 0.2f;

		D3D11_VIEWPORT vs;
		for (auto srv : _debugInfo)
		{
			topX = m_ClientWidth * (1 - 0.2 * (x + 1));
			topY = m_ClientHeight * 0.2f * y;
			vs.Height = height;
			vs.Width = width;
			vs.TopLeftX = topX;
			vs.TopLeftY = topY;
			vs.MaxDepth = 1;
			vs.MinDepth = 0;

			y++;
			if (y == 5)
			{
				x++;
				y = 0;
			}

			m_pD3dImmediateContext->RSSetViewports(1, &vs);

			m_pRenderer->Render2D(m_pD3dImmediateContext, srv);
		}
	}

	/*float topX, topY, width, height;
	width = m_ClientWidth * 0.2f;
	height = m_ClientHeight * 0.2f;
	for (int i = 1; i <= 5; i++)
	{
		topX = m_ClientWidth * 0.8f;
		topY = m_ClientHeight * 0.2f * (i - 1);

		D3D11_VIEWPORT vs;
		vs.Height = height;
		vs.Width = width;
		vs.TopLeftX = topX;
		vs.TopLeftY = topY;
		vs.MaxDepth = 1;
		vs.MinDepth = 0;

		m_pD3dImmediateContext->RSSetViewports(1, &vs);

		m_pRenderer->Render2D(m_pD3dImmediateContext, m_RTVs[i]->GetSRV());
	}

	if (m_bOnOffSSAO && m_pSSAO != nullptr)
	{
		topX = m_ClientWidth * 0.6f;
		topY = 0;

		D3D11_VIEWPORT vs;
		vs.Height = height;
		vs.Width = width;
		vs.TopLeftX = topX;
		vs.TopLeftY = topY;
		vs.MaxDepth = 1;
		vs.MinDepth = 0;

		m_pD3dImmediateContext->RSSetViewports(1, &vs);

		m_pRenderer->Render2D(m_pD3dImmediateContext, m_pSSAO->GetSSAOMap());
	}*/

	/*{
		topX = m_ClientWidth * 0.6f;
		topY = m_ClientHeight * 0.2f;

		D3D11_VIEWPORT vs;
		vs.Height = height;
		vs.Width = width;
		vs.TopLeftX = topX;
		vs.TopLeftY = topY;
		vs.MaxDepth = 1;
		vs.MinDepth = 0;

		m_pD3dImmediateContext->RSSetViewports(1, &vs);

		m_pRenderer->Render2D(m_pD3dImmediateContext, m_pPostProcess->GetOutLineRTV()->GetSRV());
	}*/

	m_pD3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);
}

HRESULT TLGraphicsEngine::GraphicsEngine::GetAdapterInfo()
{
	IDXGIAdapter1* pAdapter;
	IDXGIFactory1* pFactory = NULL;

	HRESULT hr = S_OK;

	// DXGIFactory 개체 생성(DXGI.lib 필요)
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
	{
		return E_FAIL;
	}

	hr = pFactory->EnumAdapters1(0, &pAdapter);

	if (hr != DXGI_ERROR_NOT_FOUND)
	{
		pAdapter->GetDesc1(&m_AdapterDesc1);
	}

	ReleaseCOM(pAdapter);
	ReleaseCOM(pFactory);

	return hr;
}

void TLGraphicsEngine::GraphicsEngine::CreateRenderStates()
{
	// Render State 중 Rasterizer State
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;
	solidDesc.DepthBias = 10000.f;
	solidDesc.SlopeScaledDepthBias = 1.f;
	solidDesc.DepthBiasClamp = 0.f;

	HR(m_pD3dDevice->CreateRasterizerState(&solidDesc, &m_pSolidRS));

	DX11SetObjectName(m_pSolidRS, "SolidRS");

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(m_pD3dDevice->CreateRasterizerState(&wireframeDesc, &m_pWireframeRS));

	DX11SetObjectName(m_pWireframeRS, "WireframeRS");

	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.DepthClipEnable = true;

	HR(m_pD3dDevice->CreateRasterizerState(&noCullDesc, &m_pNoCullModeRS));

	DX11SetObjectName(m_pNoCullModeRS, "NoCullModeRS");

	D3D11_RASTERIZER_DESC shadowRSDesc;
	ZeroMemory(&shadowRSDesc, sizeof(D3D11_RASTERIZER_DESC));
	shadowRSDesc.FillMode = D3D11_FILL_SOLID;
	shadowRSDesc.CullMode = D3D11_CULL_BACK;
	shadowRSDesc.FrontCounterClockwise = false;
	shadowRSDesc.DepthBias = 10000.f;
	shadowRSDesc.DepthBiasClamp = 0.0f;
	shadowRSDesc.SlopeScaledDepthBias = 1.0f;
	shadowRSDesc.DepthClipEnable = true;

	HR(m_pD3dDevice->CreateRasterizerState(&shadowRSDesc, &m_pShadowMapRS));

	DX11SetObjectName(m_pShadowMapRS, "ShadowMapRS");

	//
	// 폰트용 DSS
	//
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// 깊이버퍼에 쓰기는 한다
	equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	HR(m_pD3dDevice->CreateDepthStencilState(&equalsDesc, &m_NormalDSS));

	DX11SetObjectName(m_NormalDSS, "NormalDSS");

	//
	// 일반 DSS
	//
	D3D11_DEPTH_STENCIL_DESC nsDesc;
	ZeroMemory(&nsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	nsDesc.DepthEnable = true;
	nsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	nsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	nsDesc.StencilEnable = true;
	nsDesc.StencilReadMask = 0xff;
	nsDesc.StencilWriteMask = 0x80;

	nsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	nsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	nsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	nsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;

	nsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	nsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	nsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	nsDesc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
	HR(m_pD3dDevice->CreateDepthStencilState(&nsDesc, &m_Normal_StencilDSS));

	DX11SetObjectName(m_Normal_StencilDSS, "Normal_StencilDSS");

	//Test
	D3D11_DEPTH_STENCIL_DESC equalsDesc2;
	ZeroMemory(&equalsDesc2, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc2.DepthEnable = true;
	equalsDesc2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	equalsDesc2.DepthFunc = D3D11_COMPARISON_EQUAL;

	HR(m_pD3dDevice->CreateDepthStencilState(&equalsDesc2, &m_Normal_Equal_StencilDSS));

	DX11SetObjectName(m_Normal_Equal_StencilDSS, "Normal_Equal_StencilDSS");

	//
	// 깊이 체크 Off DSS
	//
	//D3D11_DEPTH_STENCIL_DESC equalsDesc;
	D3D11_DEPTH_STENCIL_DESC alwayDesc{};
	ZeroMemory(&alwayDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	alwayDesc.DepthEnable = false;
	alwayDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	alwayDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	HR(m_pD3dDevice->CreateDepthStencilState(&alwayDesc, &m_OffDepthCheckDSS));

	DX11SetObjectName(m_OffDepthCheckDSS, "OffDepthCheckDSS");

	//
	// MRT용 DSS
	//
	//D3D11_DEPTH_STENCIL_DESC equalsDesc;
	D3D11_DEPTH_STENCIL_DESC mrtDesc;
	ZeroMemory(&mrtDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	mrtDesc.DepthEnable = false;
	mrtDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	mrtDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	mrtDesc.StencilEnable = true;
	mrtDesc.StencilReadMask = 0x80;
	mrtDesc.StencilWriteMask = 0x00;

	mrtDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mrtDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mrtDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	mrtDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	mrtDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mrtDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mrtDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	mrtDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	HR(m_pD3dDevice->CreateDepthStencilState(&mrtDesc, &m_MRTDSS));

	DX11SetObjectName(m_MRTDSS, "MRTDSS");

	D3D11_DEPTH_STENCIL_DESC shadowDesc;
	ZeroMemory(&shadowDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	shadowDesc.DepthEnable = true;
	shadowDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	shadowDesc.DepthFunc = D3D11_COMPARISON_LESS;
	shadowDesc.StencilEnable = true;
	shadowDesc.StencilReadMask = 0xff;
	shadowDesc.StencilWriteMask = 0xff;

	shadowDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	shadowDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	shadowDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	shadowDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	shadowDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	shadowDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	shadowDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	shadowDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	HR(m_pD3dDevice->CreateDepthStencilState(&shadowDesc, &m_ShadowDSS));

	DX11SetObjectName(m_ShadowDSS, "ShadowDSS");
}
