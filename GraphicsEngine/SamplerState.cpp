#include "pch.h"
#include "SamplerState.h"
#include "GraphicsEngine.h"

TLGraphicsEngine::SamplerState* TLGraphicsEngine::SamplerState::s_pInstance = nullptr;

TLGraphicsEngine::SamplerState::SamplerState()
{

}

TLGraphicsEngine::SamplerState::~SamplerState()
{

}

void TLGraphicsEngine::SamplerState::Initalize()
{
	auto* device = GraphicsEngine::Instance()->GetDevice();

	D3D11_SAMPLER_DESC samAnisotropicWrap = {};
	samAnisotropicWrap.Filter = D3D11_FILTER_ANISOTROPIC;
	samAnisotropicWrap.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samAnisotropicWrap.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samAnisotropicWrap.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samAnisotropicWrap.MipLODBias = 0.0f;
	samAnisotropicWrap.MaxAnisotropy = 4;
	samAnisotropicWrap.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samAnisotropicWrap.MinLOD = -FLT_MAX;
	samAnisotropicWrap.MaxLOD = FLT_MAX;

	CreateSamplers(device, &samAnisotropicWrap, "samAnisotropicWrap");

	D3D11_SAMPLER_DESC samAnisotropicClamp = {};
	samAnisotropicClamp.Filter = D3D11_FILTER_ANISOTROPIC;
	samAnisotropicClamp.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samAnisotropicClamp.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samAnisotropicClamp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samAnisotropicClamp.MipLODBias = 0.0f;
	samAnisotropicClamp.MaxAnisotropy = 4;
	samAnisotropicClamp.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samAnisotropicClamp.MinLOD = -FLT_MAX;
	samAnisotropicClamp.MaxLOD = FLT_MAX;

	CreateSamplers(device, &samAnisotropicClamp, "samAnisotropicClamp");

	D3D11_SAMPLER_DESC samAnisotropicMirror = {};
	samAnisotropicMirror.Filter = D3D11_FILTER_ANISOTROPIC;
	samAnisotropicMirror.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samAnisotropicMirror.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samAnisotropicMirror.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samAnisotropicMirror.MipLODBias = 0.0f;
	samAnisotropicMirror.MaxAnisotropy = 4;
	samAnisotropicMirror.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samAnisotropicMirror.MinLOD = -FLT_MAX;
	samAnisotropicMirror.MaxLOD = FLT_MAX;

	CreateSamplers(device, &samAnisotropicMirror, "samAnisotropicMirror");

	D3D11_SAMPLER_DESC samLinearClamp = {};
	samLinearClamp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samLinearClamp.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samLinearClamp.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samLinearClamp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samLinearClamp.MipLODBias = 0.0f;
	samLinearClamp.MaxAnisotropy = 4;
	samLinearClamp.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samLinearClamp.MinLOD = -FLT_MAX;
	samLinearClamp.MaxLOD = FLT_MAX;

	CreateSamplers(device, &samLinearClamp, "samLinearClamp");

	D3D11_SAMPLER_DESC samLinearWrap = {};
	samLinearWrap.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samLinearWrap.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samLinearWrap.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samLinearWrap.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samLinearWrap.MipLODBias = 0.0f;
	samLinearWrap.MaxAnisotropy = 4;
	samLinearWrap.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samLinearWrap.MinLOD = -FLT_MAX;
	samLinearWrap.MaxLOD = FLT_MAX;

	CreateSamplers(device, &samLinearWrap, "samLinearWrap");

	D3D11_SAMPLER_DESC samLinearMirror = {};
	samLinearMirror.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samLinearMirror.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samLinearMirror.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samLinearMirror.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samLinearMirror.MipLODBias = 0.0f;
	samLinearMirror.MaxAnisotropy = 4;
	samLinearMirror.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samLinearMirror.MinLOD = -FLT_MAX;
	samLinearMirror.MaxLOD = FLT_MAX;

	CreateSamplers(device, &samLinearMirror, "samLinearMirror");

	D3D11_SAMPLER_DESC samPointBorder = {};
	samPointBorder.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samPointBorder.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samPointBorder.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samPointBorder.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samPointBorder.MipLODBias = 0.0f;
	samPointBorder.MaxAnisotropy = 4;
	samPointBorder.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samPointBorder.MinLOD = -FLT_MAX;
	samPointBorder.MaxLOD = FLT_MAX;
	samPointBorder.BorderColor[0] = 0.0f;
	samPointBorder.BorderColor[1] = 0.0f;
	samPointBorder.BorderColor[2] = 1e5f;
	samPointBorder.BorderColor[3] = 1e5f;

	CreateSamplers(device, &samPointBorder, "samPointBorder");

	D3D11_SAMPLER_DESC samPointWrap = {};
	samPointWrap.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samPointWrap.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samPointWrap.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samPointWrap.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samPointWrap.MipLODBias = 0.0f;
	samPointWrap.MaxAnisotropy = 4;
	samPointWrap.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samPointWrap.MinLOD = -FLT_MAX;
	samPointWrap.MaxLOD = FLT_MAX;
	samPointWrap.BorderColor[0] = 0.0f;
	samPointWrap.BorderColor[1] = 0.0f;
	samPointWrap.BorderColor[2] = 0.0f;
	samPointWrap.BorderColor[3] = 1e5f;

	CreateSamplers(device, &samPointWrap, "samPointWrap");

	D3D11_SAMPLER_DESC samShadow = {};
	samShadow.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samShadow.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samShadow.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samShadow.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samShadow.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samShadow.MinLOD = -FLT_MAX;
	samShadow.MaxLOD = FLT_MAX;
	samPointWrap.BorderColor[0] = 0.0f;
	samPointWrap.BorderColor[1] = 0.0f;
	samPointWrap.BorderColor[2] = 0.0f;
	samPointWrap.BorderColor[3] = 0.0f;

	CreateSamplers(device, &samShadow, "samShadow");
}

bool TLGraphicsEngine::SamplerState::CreateSamplers(ID3D11Device* device, D3D11_SAMPLER_DESC* desc, std::string samplerName)
{
	auto item = m_SamplerStateMap.find(samplerName);

	if (item != m_SamplerStateMap.end())
	{
#ifdef _DEBUG
		std::string error = samplerName + " 샘플러가 이미 존재합니다.";
		assert(false && error.c_str());
#endif // _DEBUG
		return false;
	}

	ID3D11SamplerState* newSamplerState = nullptr;
	
	HRESULT hr = device->CreateSamplerState(desc, &newSamplerState);

#ifdef _DEBUG
	DX11SetObjectName(newSamplerState, samplerName.c_str());
#endif // _DEBUG

	if (SUCCEEDED(hr))
	{
		m_SamplerStateMap.insert(make_pair(samplerName, newSamplerState));
	}
	else
	{
#ifdef _DEBUG
		std::string error = samplerName + " 샘플러를 만드는데 실패.";
		assert(false && error.c_str());
#endif // _DEBUG
		return false;
	}

	return true;
}

TLGraphicsEngine::SamplerState* TLGraphicsEngine::SamplerState::Instance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new SamplerState;
		s_pInstance->Initalize();
	}

	return s_pInstance;
}

void TLGraphicsEngine::SamplerState::Finalize()
{
	if (s_pInstance != nullptr)
	{
		delete s_pInstance;
		s_pInstance = nullptr;
	}
}

ComPtr<ID3D11SamplerState> TLGraphicsEngine::SamplerState::GetSamplerState(std::string name)
{
	auto item = m_SamplerStateMap.find(name);

	if (item != m_SamplerStateMap.end())
	{
		return item->second;
	}

#ifdef _DEBUG
	std::string error = name + "에 해당하는 샘플러를 찾지 못했음";
	assert(false && error.c_str());
#endif // _DEBUG
	return nullptr;
}
