#pragma once

#include "ComPtr.h"

struct ID3D11SamplerState;

namespace TLGraphicsEngine
{
	class SamplerState
	{
	private:
		SamplerState();
		SamplerState(const SamplerState& sampler) = delete;
		
	public:
		~SamplerState();


	private:
		static SamplerState* s_pInstance;
		std::unordered_map<std::string, ComPtr<ID3D11SamplerState>> m_SamplerStateMap;

	private:
		void Initalize();
	public:
		static SamplerState* Instance();
		static void Finalize();

		bool CreateSamplers(ID3D11Device* device, D3D11_SAMPLER_DESC* desc, std::string samplerName);

		ComPtr<ID3D11SamplerState> GetSamplerState(std::string name);
	};
}