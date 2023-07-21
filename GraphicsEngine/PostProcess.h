#pragma once

#include "Export/ResourceBase.h"

const int g_maxDownSampleCount = 4;

struct ID3D11DeviceContext;
struct ID3DX11EffectTechnique;

namespace TLGraphicsEngine
{
	class PPMaterial;
	class RenderTargetView;
	class DepthStencilView;
	class Shader;
	struct RenderObject;

	class PostProcess
	{
	public:
		PostProcess();
		~PostProcess();

	private:
		shared_ptr<Shader> m_PostProcessVS;
		shared_ptr<Shader> m_PostProcessBlurPS;
		shared_ptr<Shader> m_PostProcessBloomPS;
		shared_ptr<Shader> m_PostProcessDOFPS;
		shared_ptr<Shader> m_PostProcessToneMapPS;
		shared_ptr<Shader> m_PostProcessOutLineVS;
		shared_ptr<Shader> m_PostProcessOutLinePS;
		shared_ptr<Shader> m_PostProcessOutLineAddPS;

		shared_ptr<Shader> m_PostProcessDownPS;
		shared_ptr<Shader> m_PostProcessUpPS;
		shared_ptr<Shader> m_PostProcessAddPS;

		Shader* m_CurrEffect = nullptr;

		RenderTargetView* m_TargetRTV = nullptr;

		std::vector<RenderTargetView*> m_DownSamplings;
		std::vector<RenderTargetView*> m_DownSamplings2;

		RenderTargetView* m_OutLineRTV;
		DepthStencilView* m_OutLineDSV;

		bool m_bOnOffBlur = false;
		bool m_bOnOffBloom = false;
		bool m_bOnOffToneMapping = false;

		float m_fThresholdMin = 1.0f;
		float m_fThresholdMax = 1.26f;
		float m_fExposure = 2.f;

	public:
		RenderTargetView* GetOutLineRTV() { return m_OutLineRTV; }
		DepthStencilView* GetOutLineDSV() { return m_OutLineDSV; }

	public:

		void Initialize();

		void OnOffBlur(bool value) { m_bOnOffBlur = value; }
		void OnOffBloom(bool value) { m_bOnOffBloom = value; }
		virtual void SetBloomThreshold(float min, float max) { m_fThresholdMin = min; m_fThresholdMax = max; }
		void OnOffToneMapping(bool value) { m_bOnOffToneMapping = value; }
		virtual void SetToneMapExposure(float value) { m_fExposure = value; }

		/// <summary>
		/// srv 원본을 포스트 프로세싱 적용후 rtv에 최종 출력
		/// </summary>
		/// <param name="srv">포스트 프로세싱을 적용시킬 srv</param>
		/// <param name="rtv">이펙트 적용후 출력할 rtv</param>
		RenderTargetView* PostProcessPass(RenderTargetView* srv, RenderTargetView* rtv);

		/// <summary>
		/// Blur 처리하기 위한 셋팅
		/// </summary>
		/// <param name="srv"></param>
		/// <param name="rtv"></param>
		void Blur( RenderTargetView* srv, RenderTargetView* rtv);

		/// <summary>
		/// Bloom 처리하기 위한 셋팅
		/// </summary>
		/// <param name="srv"></param>
		/// <param name="rtv"></param>
		void Bloom(RenderTargetView* srv, RenderTargetView* rtv);

		/// <summary>
		/// HDR Image 처리하기 위한 셋팅
		/// </summary>
		/// <param name="srv"></param>
		/// <param name="rtv"></param>
		void ToneMap(RenderTargetView* srv, RenderTargetView* rtv);

		/// <summary>
		/// Depth Of Field 처리하기 위한 셋팅
		/// </summary>
		void DOF(RenderTargetView* srv, RenderTargetView* rtv);
		
		/// <summary>
		/// OutLine 텍스처
		/// </summary>
		/// <param name="main"></param>
		/// <param name="depth"></param>
		/// <param name="rtv"></param>
		void OutLine(RenderTargetView* rtv);
		void AddOutLine(RenderTargetView* srv, RenderTargetView* rtv);

		/// <summary>
		/// 현재 적용된 Post Effect를 적용시킴
		/// </summary>
		void SetEffect(ID3D11DeviceContext* deviceContext);

		/// <summary>
		/// 다운 샘플링할 렌더 타겟 뷰들의 사이즈 재조정
		/// </summary>
		/// <param name="width">Window Width</param>
		/// <param name="height">Window Height</param> 
		void OnResize(UINT width, UINT height);

		void BloomCurve(ID3D11DeviceContext* deviceContext, RenderTargetView* srv, RenderTargetView* rtv);
		void SetBlur(ID3D11DeviceContext* deviceContext, RenderTargetView* srv, RenderTargetView* rtv);
		void SetBlurAxis(ID3D11DeviceContext* deviceContext, int axis, RenderTargetView* srv, RenderTargetView* rtv);
		void DownSampling(ID3D11DeviceContext* deviceContext, RenderTargetView* srv, RenderTargetView* rtv);
		void UpSampling(ID3D11DeviceContext* deviceContext, RenderTargetView* downSRV, RenderTargetView* upSRV, RenderTargetView* rtv, float blendDown, float blendUp);
		void AddSampling(ID3D11DeviceContext* deviceContext, RenderTargetView* downSRV, RenderTargetView* upSRV, RenderTargetView* rtv);


	private:
		void UnBindResource(ID3D11DeviceContext* deviceContext, int srvCnt, int rtvCnt);
	};
}

