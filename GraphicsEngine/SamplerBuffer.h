#pragma once
#include "ShaderResourceBase.h"

/// <summary>
/// Sampler Resource
/// </summary>

struct ID3D11SamplerState;

namespace TLGraphicsEngine
{
	enum class ShaderType;

	class SamplerBuffer :
	    public ShaderResourceBase
	{
	public:
		SamplerBuffer(const char* name, int bindPoint, int bindCnt);
		virtual ~SamplerBuffer();

	private:
		/// <summary>
		/// 쉐이더에서 받아온 샘플러 이름을 통해 샘플러스테이트 클래스에서 이미 생성한
		/// 샘플러스테이트를 찾아옴
		/// </summary>
		ComPtr<ID3D11SamplerState> m_pSamplerState;
		
	public:

		void Update(ShaderType type);

		// 외부에서 샘플러를 받아서 쓰고 싶을때 사용 사용예정은 딱히 없음
		void Update(ShaderType type, ID3D11SamplerState* sampler);
	};
}