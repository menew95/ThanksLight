#pragma once

#include "Export/ResourceBase.h"
#include "DX11Define.h"

namespace TLGraphicsEngine
{
	class RenderTargetView;
	class Shader;
	class SSAOMaterial;

	/// <summary>
	/// SSAO PostProcess Pass
	/// </summary>

	class SSAO
	{
	public:
		SSAO();
		~SSAO();
		
		void Initialize();

	private:
		RenderTargetView*	m_SSAORTV[2]	= { nullptr, nullptr };
		shared_ptr<Shader>	m_SSAOVS		= nullptr;
		shared_ptr<Shader>	m_SSAOPS		= nullptr;
		shared_ptr<Shader>	m_SSAOBlurVS	= nullptr;
		shared_ptr<Shader>	m_SSAOBlurPS	= nullptr;

		UINT m_Width, m_Height;

		ComPtr<ID3D11ShaderResourceView> m_RandomVectorSRV = nullptr;

	public:
		DirectX::XMFLOAT4	m_FrustumFarCorners[4] = { };

		// 랜덤한 방향의 벡터, 수를 많이 할 수록 좀더 퀼리티가 높아 진다.
		DirectX::XMFLOAT4	m_Offsets[14] = { };

	private:
		void BuildFrustumFarCorners(float fovY, float farZ, float aspect);
		void BuildOffsetVectors();
		void BuildRandomVectorTexture();

		void Blur();

	public:
		void OnResize(UINT width, UINT height);

		// 카메라 셋팅이 바뀔때 새롭게 SSAO 셋팅을 해줘야한다.
		// 지금은 일단 BeginRender에서 매 프레임 해주는중, 추후 카메라 셋팅이 바뀔때만 호출하도록 변경필요
		void Setting(float fovY, float farZ, float aspect);



		void OnSSAO(ID3D11ShaderResourceView* normalSRV, ID3D11ShaderResourceView* depthSRV);
	
		ID3D11ShaderResourceView* GetSSAOMap();
		ID3D11ShaderResourceView** GetSSAOMapRef();
	};
}

