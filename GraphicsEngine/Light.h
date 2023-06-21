#pragma once

struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

namespace TLGraphicsEngine
{
	struct RenderObject;
	class DepthStencilView;
	class RenderTargetView;

	enum class LightType
	{
		Spot		= 0,
		Directional	= 1,
		Point		= 2,
		BlackSpotLight = 3
	};

	/// <summary>
	/// 
	/// </summary>
	class Light
	{
	public:
		Light();
		~Light();

	public:
		void BuildShadowMap();

		ID3D11ShaderResourceView** GetShadowMapRef();
		bool CheckLight(RenderObject*);

		void BindShadowMap(ID3D11DeviceContext* deviceContext);
		void BindShadowMapTexture(ID3D11DeviceContext* deviceContext);

		DirectX::XMMATRIX GetViewProjTM();

		DepthStencilView* GetDepthBuffer() { return m_shadowMapDepthStencil; }

	private:
		bool CheckDirectionalLight(RenderObject*);
		bool CheckPointLight(RenderObject*);
		bool CheckSpotLight(RenderObject*);

		void DirectionalLightViewProjTM();
		void SpotLightViewProjTM();

	public:
		bool m_enable = true;

		LightType m_eLightType = LightType::Point;

		float m_range = 0.f;	// point / spot
		float m_fallOff = 0.f;	// point / spot
		float m_spotAngle = 0.f;	// spot / Radian
		float m_fallOffAngle = 1.f;	// spot / Radian
		float m_power = 1.f;

		DirectX::XMFLOAT3 m_lightPosition;
		DirectX::XMFLOAT3 m_lightDirection;

		DirectX::XMFLOAT3 m_color;

		DepthStencilView* m_shadowMapDepthStencil;
		RenderTargetView* m_shadowMapRenderTarget;

		DirectX::XMFLOAT3 m_center;
		float m_radius;

		DirectX::XMMATRIX m_shadowTM;
		DirectX::XMMATRIX m_viewTM;
		DirectX::XMMATRIX m_projTM;
		DirectX::XMMATRIX m_viewProjTM;
	};
}