#pragma once

#include "Export/IGraphicsEngine.h"
#include "DX11Define.h"

using namespace DirectX;
class DXTKFont;

namespace TLGraphicsEngine
{
	class EngineCamera;
	class IMesh;
	class Mesh;
	class IMaterial;
	class Material;
	class Shader;
	class Texture;
	class TextMeta;
	class GraphicResourceManager;
	class RenderQueue;
	class Frustum;
	class SkyBox;
	class RenderTargetView;
	class DepthStencilView;
	class PostProcess;
	class ProjecedTexturePass;
	class SSAO;
	class LightManager;
	class UIRenderer;
	class Renderer;
	struct RenderObject;
	struct Light_CB;
	/// <summary>
	/// Graphic Engine
	/// 2022.08.01
	/// </summary>

	class GraphicsEngine :
	    public IGraphicsEngine
	{
	public:
		GraphicsEngine();
		~GraphicsEngine();

	private:
		static GraphicsEngine* m_graphicInstance;
	
	public:
		static GraphicsEngine* Instance();

		virtual void BeginRender();
		virtual void Render();
		virtual void EndRender();

		// 엔진을 종료한다.
		virtual void Finalize();

		// 초기화 관련
		virtual void InitializeGraphic(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight);
		virtual void OnResize();
		virtual void SetResize(UINT width, UINT height, WPARAM wParam);

		// RenderQueue
		virtual void RegistRenderQueue(IMesh* mesh, std::vector<IMaterial*> mats, float WM[16], int renderType, float* m_BoneTMList = nullptr, int size = 0);
		virtual void RegistRenderQueue(UID meshUID, std::vector<UID> matUIDs, float WM[16], int renderType, float* m_BoneTMList = nullptr, int size = 0);
		virtual void RegistTextUIRenderQueue(float x, float y, const char* text, float* color, int fontSize, float lineSpacing, float alignX, float alignY);
		virtual void RegistImageUIRenderQueue(float x, float y, ITexture* texture, float* color, RECT rect, int imageType, float value);

		virtual void RegistGizmo(int gizmoType, float WM[16], float* color);

		// Camera Control
		virtual void SetCameraPosition(float posX, float posY, float posZ);
		virtual void SetCameraLookAt(float lookX, float lookY, float lookZ);
		virtual void SetCameraUp(float upX, float upY, float upZ);
		virtual void SetCameraRight(float rightX, float rightY, float rightZ);
		virtual void SetLookAt(float posX, float posY, float posZ,
			float targetX, float targetY, float targetZ,
			float worldUpX, float worldUpY, float worldUpZ);

		virtual void SetLens(float fovY, float zn, float zf);
		virtual void FrustumUpdate(float posx, float posy, float posz, float forwardx,
			float forwardy, float forwardz, float upx, float upy, float upz,
			float fovY, float aspectRatio, float nearZ, float farZ);

		/// 리소스 관련
		virtual IMesh* GetIMesh(const char* file);
		virtual ITexture* GetITexture(const char* file);
		virtual IMaterial* GetIMaterial(const char* file);
		virtual Shader* GetShader(const char* file);
		virtual IMesh* GetIMesh(UID uid);
		virtual ITexture* GetITexture(UID uid);
		virtual IMaterial* GetIMaterial(UID uid);
		virtual Shader* GetShader(UID uid);

		virtual std::shared_ptr<IMesh> GetIMeshRef(const char* file);
		virtual std::shared_ptr<ITexture> GetITextureRef(const char* file);
		virtual std::shared_ptr<IMaterial> GetIMaterialRef(const char* file);
		virtual std::shared_ptr<Shader> GetShaderRef(const char* file);
		virtual std::shared_ptr<IMesh> GetIMeshRef(UID uid);
		virtual std::shared_ptr<ITexture> GetITextureRef(UID uid);
		virtual std::shared_ptr<IMaterial> GetIMaterialRef(UID uid);
		virtual std::shared_ptr<Shader> GetShaderRef(UID uid);

		virtual std::shared_ptr<IMesh> CreateMesh(const char* name, UID uid);
		virtual std::shared_ptr<IMaterial> CreateMaterial(const char* name, UID uid);
		virtual std::shared_ptr<ITexture> CreateTexture(const char* name, UID uid);
		virtual std::shared_ptr<Shader> CreateShader(const char* name, UID uid);

		virtual void LoadPrefab(IPrefabInfo& prefab, const char* file);

		// 피킹 테스트 관련
		virtual void OnMouseMove(int screenX, int screenY);

		// SSAO 옵션
		virtual void SetSSAO(bool value);
		virtual void SetBlur(bool value);
		virtual void SetBloom(bool value);
		virtual void SetBloomThreshold(float min, float max);

		virtual void SetToneMap(bool value);
		virtual void SetToneMapExposure(float value);
		virtual void SetIBLFactor(float value);

		// Light 관련
		virtual int AddLight();
		virtual void DeleteLight(int id);
		virtual void SetLightEnable(int id, bool enable);
		virtual void UpdateLight(int id, int type, float range, float fallOff, float spotAngle, float fallOffAngle, float power, float* pos, float* dir, float* color);
		virtual void ClearLight();

		virtual void OnOffDebug();
		virtual void OnDebug(int type);

		bool m_OnOffDebug = false;
		bool m_gizmo = false;
		bool m_mrt = false;
		bool m_postProcess = false;
	public:
		void CreateMeshBuffer(std::shared_ptr<Mesh> mesh);

		void CheckPicking(Mesh* mesh, XMMATRIX meshWorld);

		void UpdateFrameConstBuffer();

		void GetLightBuffer(Light_CB* lightcb, std::vector<ID3D11ShaderResourceView**>& srvs);

	public:
		///  캡슐화
		virtual bool GetEnginePause() { return m_isGraphicEnginePause; }
		virtual void SetEnginePause(bool value) { m_isGraphicEnginePause = value; }
		virtual bool GetMinimized() { return m_isMinimized; }
		virtual void SetMinimized(bool value) { m_isMinimized = value; }
		virtual bool GetMaximized() { return m_isMaximized; }
		virtual void SetMaximized(bool value) { m_isMaximized = value; }
		virtual bool GetResizing() { return m_isResizing; }
		virtual void SetResizing(bool value) { m_isResizing = value; }

		ID3D11RasterizerState* GetWireframeRS() { return m_pWireframeRS; };
		ID3D11RasterizerState* GetSolidRS() { return m_pSolidRS; };
		ID3D11RasterizerState* GetNoCullModeRS() { return m_pNoCullModeRS; };
		ID3D11RasterizerState* GetShadowMapRS() { return m_pShadowMapRS; };

		ID3D11DepthStencilState* GetNormalDSS() { return m_NormalDSS; };
		ID3D11DepthStencilState* GetOffDepthCheckDSS() { return m_OffDepthCheckDSS; };

		ID3D11Device* GetDevice() { return m_pD3dDevice; };
		ID3D11DeviceContext* GetDeviceContext() { return m_pD3dImmediateContext; };
		DepthStencilView* GetDepthStencilView() { return m_pDepthStencilView; }

		EngineCamera* GetEngineCamera() { return m_EngineCamera; }

		PostProcess* GetPostProcess() { return m_pPostProcess; }

		int GetClientWidth() { return m_ClientWidth; }
		int GetClientHeight() { return m_ClientHeight; }

		float GetAspectRatio() { return static_cast<float>(m_ClientWidth) / m_ClientHeight; }
	private:

		void DrawText(int x, int y, float r, float g, float b, float a, TCHAR* pch, ...);

		// MRT를 하나로 합치는 작업
		void MRTRender();

		/// <summary>
		/// 화면 출력
		/// </summary>
		/// <param name="rtv"></param>
		void RenderToMain(RenderTargetView* rtv);

		std::vector<ID3D11ShaderResourceView**> shadowMapDebug;

		HRESULT GetAdapterInfo();

		void CreateRenderStates();
	private:
		bool							m_isGraphicEnginePause;
		bool							m_isMinimized;
		bool							m_isMaximized;
		bool							m_isResizing;
		UINT							m_4xMsaaQuality = 0;

		ID3D11Device* m_pD3dDevice = nullptr;
		ID3D11DeviceContext* m_pD3dImmediateContext = nullptr;
		IDXGISwapChain* m_pSwapChain = nullptr;
		ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
		std::vector<RenderTargetView*>	m_RTVs;
		DepthStencilView* m_pDepthStencilView = nullptr;
		D3D11_VIEWPORT					m_ScreenViewport = { 0 };

		float							m_fTheta;
		float							m_fPhi;
		float							m_fRadius;

		D3D_DRIVER_TYPE					m_D3DDriverType = D3D_DRIVER_TYPE_HARDWARE;

		bool							m_bEnable4xMsaa = false;

		DXGI_ADAPTER_DESC1				m_AdapterDesc1 = { 0 };

		UIRenderer* m_pUIRenderer = nullptr;
		RenderQueue* m_pRenderQueue = nullptr;
		Renderer* m_pRenderer = nullptr;
		Frustum* m_pFrustum = nullptr;
		SkyBox* m_pSkyBox = nullptr;
		PostProcess* m_pPostProcess = nullptr;
		SSAO* m_pSSAO = nullptr;
		LightManager* m_pLightManager = nullptr;
		ProjecedTexturePass* m_pProjecedTexturePass = nullptr;

		bool							m_bOnOffSSAO = true;
		bool							m_bOnOffBloom = true;
		bool							m_bOnOffToneMap = true;
		float							m_fIBLFactor = 0.1f;
		int								m_iDrawObjCnt = 0;

		ID3D11RasterizerState* m_pWireframeRS = nullptr;
		ID3D11RasterizerState* m_pSolidRS = nullptr;
		ID3D11RasterizerState* m_pNoCullModeRS = nullptr;
		ID3D11RasterizerState* m_pShadowMapRS = nullptr;

		// 폰트때문에 뎁스스탠실 스테이트가 강제가 됐다.
		ID3D11DepthStencilState* m_NormalDSS = nullptr;
		ID3D11DepthStencilState* m_Normal_StencilDSS = nullptr;
		ID3D11DepthStencilState* m_Normal_Equal_StencilDSS = nullptr;
		ID3D11DepthStencilState* m_ShadowDSS = nullptr;
		ID3D11DepthStencilState* m_OffDepthCheckDSS = nullptr;
		ID3D11DepthStencilState* m_MRTDSS = nullptr;

		HINSTANCE						m_HInstance = 0;
		HWND							m_Hwnd = 0;
		HRESULT							m_DirectX11Ready = S_OK;

		UINT							m_ClientWidth = 0;
		UINT							m_ClientHeight = 0;

		EngineCamera* m_EngineCamera = nullptr;
		GraphicResourceManager* m_ResourceManager = nullptr;

		int								m_PickingCnt = 0;
		float							m_NDCMousePosX = 0.f;
		float							m_NDCMousePosY = 0.f;

		bool m_needRecoredDepth = false;
	};
}
