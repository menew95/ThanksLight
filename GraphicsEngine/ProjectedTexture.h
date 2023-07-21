#pragma once

struct ID3D11DeviceContext;

namespace TLGraphicsEngine
{
	class RenderTargetView;
	class DepthStencilView;

	class ProjectedTexture
	{
	public:
		ProjectedTexture();
		~ProjectedTexture();

	public:

		void BindRTV(ID3D11DeviceContext* deviceContext);
		void BindDepth(ID3D11DeviceContext* deviceContext);

		void OnResize(int width, int height);

		void SetDirty(bool value) { m_isDirty = value; }
		bool IsDirty() { return m_isDirty; }
		void SetOnOff(bool value) { m_onOff = value; }
		bool GetOnOff() { return m_onOff; }

		void SetIsSetObject(bool value) { m_isSetObject = value; }
		bool GetIsSetObject() { return m_isSetObject; }

		XMFLOAT4X4 GetMatrix() { return m_cameraViewProj;  }

		RenderTargetView* GetRenderTargetView() { return m_renderTargetView; }
		//RenderTargetView* GetDepthStencilView() { return m_depth; }
		DepthStencilView* GetDepthStencilView() { return m_depth; }

		void Reset();
	private:
		bool m_isSetObject = false;
		bool m_isDirty = false;
		bool m_onOff = false;
		XMFLOAT4X4 m_cameraViewProj;
		RenderTargetView* m_renderTargetView;
		//RenderTargetView* m_depth;
		DepthStencilView* m_depth;
	};
}