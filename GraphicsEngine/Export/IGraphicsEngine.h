#pragma once

#include <windows.h>
#include <memory>
#include <string>
#include <vector>
#ifdef GRAPHICENGINE_EXPORTING
#define GRAPHIC_CLASS_DECLSPEC    __declspec(dllexport)
#define GRAPHIC_DLL_TEMPLATE
using UID = std::string;
#else
#define GRAPHIC_CLASS_DECLSPEC    __declspec(dllimport)
#define GRAPHIC_DLL_TEMPLATE extern
#endif

struct IPrefabInfo;


namespace TLGraphicsEngine
{
	class IMesh;
	class IMaterial;
	class ITexture;
	class Shader;

	enum class eEngineType
	{
		DirectX11 = 0
	};

	class GRAPHIC_CLASS_DECLSPEC IGraphicsEngine abstract
	{
	public:
		virtual void BeginRender() abstract;
		virtual void Render() abstract;
		virtual void EndRender() abstract;
		// 엔진을 종료한다.
		virtual void Finalize() abstract;

		// 초기화 관련
		virtual void InitializeGraphic(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight) abstract;
		virtual void OnResize() abstract;
		virtual void SetResize(UINT width, UINT height, WPARAM wParam) abstract;

		// RenderQueue
		virtual void RegistRenderQueue(IMesh* mesh, std::vector<IMaterial*> mats, float WM[16], int renderType, float* m_BoneTMList = nullptr, int size = 0) abstract;
		virtual void RegistRenderQueue(UID meshUID, std::vector<UID> matUIDs, float WM[16], int renderType, float* m_BoneTMList = nullptr, int size = 0) abstract;
		virtual void RegistTextUIRenderQueue(float x, float y, const char* text, float* color, int fontSize, float lineSpacing, float alignX, float alignY) abstract;
		virtual void RegistImageUIRenderQueue(float x, float y, ITexture* texture, float* color, RECT rect, int imageType, float value) abstract;
		virtual void RegistGizmo(int gizmoType, float WM[16], float* color) abstract;

		// Camera Control
		virtual void SetCameraPosition(float posX, float posY, float posZ) abstract;
		virtual void SetCameraLookAt(float lookX, float lookY, float lookZ) abstract;
		virtual void SetCameraUp(float upX, float upY, float upZ) abstract;
		virtual void SetCameraRight(float rightX, float rightY, float rightZ) abstract;
		virtual void SetLookAt(float posX, float posY, float posZ,
			float targetX, float targetY, float targetZ,
			float worldUpX, float worldUpY, float worldUpZ) abstract;

		virtual void SetLens(float fovY, float zn, float zf) abstract;
		virtual void FrustumUpdate(float posx, float posy, float posz, float forwardx,
			float forwardy, float forwardz, float upx, float upy, float upz,
			float fovY, float aspectRatio, float nearZ, float farZ) abstract;

		/// 리소스 관련
		virtual IMesh* GetIMesh(const char* file) abstract;
		virtual ITexture* GetITexture(const char* file) abstract;
		virtual IMaterial* GetIMaterial(const char* file) abstract;
		virtual Shader* GetShader(const char* file) abstract;
		virtual IMesh* GetIMesh(UID uid) abstract;
		virtual ITexture* GetITexture(UID uid) abstract;
		virtual IMaterial* GetIMaterial(UID uid) abstract;
		virtual Shader* GetShader(UID uid) abstract;

		virtual std::shared_ptr<IMesh> GetIMeshRef(const char* file) abstract;
		virtual std::shared_ptr<ITexture> GetITextureRef(const char* file) abstract;
		virtual std::shared_ptr<IMaterial> GetIMaterialRef(const char* file) abstract;
		virtual std::shared_ptr<Shader> GetShaderRef(const char* file) abstract;
		virtual std::shared_ptr<IMesh> GetIMeshRef(UID uid) abstract;
		virtual std::shared_ptr<ITexture> GetITextureRef(UID uid) abstract;
		virtual std::shared_ptr<IMaterial> GetIMaterialRef(UID uid) abstract;
		virtual std::shared_ptr<Shader> GetShaderRef(UID uid) abstract;

		virtual std::shared_ptr<IMesh> CreateMesh(const char* name, UID uid) abstract;
		virtual std::shared_ptr<IMaterial> CreateMaterial(const char* name, UID uid) abstract;
		virtual std::shared_ptr<ITexture> CreateTexture(const char* name, UID uid) abstract;
		virtual std::shared_ptr<Shader> CreateShader(const char* name, UID uid) abstract;

		virtual void LoadPrefab(IPrefabInfo& prefab, const char* file) abstract;

		// 피킹 테스트 관련
		virtual void OnMouseMove(int screenX, int screenY) abstract;

		// SSAO 옵션
		virtual void SetSSAO(bool value) abstract;
		virtual void SetBlur(bool value) abstract;
		virtual void SetBloom(bool value) abstract;
		virtual void SetBloomThreshold(float min, float max) abstract;

		virtual void SetToneMap(bool value) abstract;
		virtual void SetToneMapExposure(float value) abstract;
		virtual void SetIBLFactor(float value) abstract;
		// Light 관련
		virtual int AddLight() abstract;
		virtual void DeleteLight(int id) abstract;
		virtual void SetLightEnable(int id, bool enable) abstract;
		virtual void UpdateLight(int id, int type, float range, float fallOff, float spotAngle, float fallOffAngle, float power, float* pos, float* dir, float* color) abstract;

		virtual void ClearLight() abstract;

		virtual void OnOffDebug() abstract;
		virtual void OnDebug(int type) abstract;
	public:
		///  캡슐화
		virtual bool GetEnginePause() abstract;
		virtual void SetEnginePause(bool value) abstract;
		virtual bool GetMinimized() abstract;
		virtual void SetMinimized(bool value) abstract;
		virtual bool GetMaximized() abstract;
		virtual void SetMaximized(bool value) abstract;
		virtual bool GetResizing() abstract;
		virtual void SetResizing(bool value) abstract;
	};

	extern "C"
	{
		GRAPHIC_CLASS_DECLSPEC IGraphicsEngine* GraphicsEngineInstance(eEngineType engineType);
		GRAPHIC_CLASS_DECLSPEC void GraphicsEngineRelease(IGraphicsEngine*);
		using GraphicsEngineConstructor = IGraphicsEngine * (*)(eEngineType engineType);
		using GraphicsEngineDestructor = void (*)(IGraphicsEngine*);
	}
}