#include "pch.h"
#include "Shader.h"
#include "GraphicsEngine.h"
#include "GraphicResourceManager.h"
#include "ConstBuffer.h"
#include "ShaderResourceBuffer.h"
#include "SamplerBuffer.h"


#include "PixelShaderBuffer.h"
#include "VertexShaderBuffer.h"
#include "GeometryShaderBuffer.h"
#include "HullShaderBuffer.h"
#include "ComputeShaderBuffer.h"
#include "DomainShaderBuffer.h"

#include "ConstBufferManager.h"

using namespace TLGraphicsEngine;

Shader::Shader(const char* name) : ResourceBase(eResourceType::Shader, name)
{

}

Shader::~Shader()
{
}

bool Shader::Update(CBBase** const cbs, ID3D11ShaderResourceView*** srvs)
{
	auto* deviceContext = GraphicsEngine::Instance()->GetDeviceContext();


	for(size_t i = 0; i < m_ConstBufferList.size(); i++)
	{
		m_ConstBufferList[i]->Update(m_ShaderType, *(cbs + i));
	}
	for (size_t i = 0; i < m_ShaderResourceList.size(); i++)
	{
		m_ShaderResourceList[i]->Update(m_ShaderType, *(srvs + i));
	}
	 
	for (size_t i = 0; i < m_SamplerList.size(); i++)
	{
		m_SamplerList[i]->Update(m_ShaderType);
	}

	return true;
}

bool Shader::Update(CBBase** const cbs, std::vector<ID3D11ShaderResourceView**>& srvs)
{
	auto* deviceContext = GraphicsEngine::Instance()->GetDeviceContext();


	for (size_t i = 0; i < m_ConstBufferList.size(); i++)
	{
		m_ConstBufferList[i]->Update(m_ShaderType, *(cbs + i));
	}
	for (size_t i = 0; i < m_ShaderResourceList.size(); i++)
	{
		if (i < srvs.size()) m_ShaderResourceList[i]->Update(m_ShaderType, srvs[i]);
	}

	for (size_t i = 0; i < m_SamplerList.size(); i++)
	{
		m_SamplerList[i]->Update(m_ShaderType);
	}

	return true;
}

bool Shader::Update(std::vector<CBBase*>& cbs, std::vector<ID3D11ShaderResourceView**>& srvs)
{
	auto* deviceContext = GraphicsEngine::Instance()->GetDeviceContext();


	for (size_t i = 0; i < m_ConstBufferList.size(); i++)
	{
		if (i < cbs.size()) m_ConstBufferList[i]->Update(m_ShaderType, cbs[i]);
	}
	for (size_t i = 0; i < m_ShaderResourceList.size(); i++)
	{
		if (i < srvs.size()) m_ShaderResourceList[i]->Update(m_ShaderType, srvs[i]);
	}

	for (size_t i = 0; i < m_SamplerList.size(); i++)
	{
		m_SamplerList[i]->Update(m_ShaderType);
	}

	return true;
}

bool Shader::ComfileBuffer(const wchar_t* wPath,/* const D3D_SHADER_MACRO* pDefines,*/ LPCSTR entry_point, LPCSTR shader_model)
{
	HRESULT hr = S_OK;
	// DirectX에서는 행렬 순서가 row-major(행우선)이고, HLSL에서는 column-major(열우선)이다.
	// 출처: https://nickchooshin.tistory.com/7 [닉추신의 작은 세계:티스토리]
	// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-appendix-pre-pragma-pack-matrix
	// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/d3dcompile-constants
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG; // 쉐이더 디버깅할 때 필요

	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* errorBlob = nullptr;

	hr = D3DCompileFromFile(
		wPath,
		nullptr,/*pDefines,*/
		nullptr,
		entry_point,
		shader_model,
		dwShaderFlags,
		0,
		&m_ShaderBlob,
		&errorBlob);

	if (FAILED(hr) == true)
	{
		if (errorBlob != nullptr)
		{
			const char* errorStr = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
			errorBlob->Release();
			throw errorStr;
		}
		else
		{
			throw _T("Shader Compile Failed\n");
		}
	}

	if (errorBlob != nullptr)
		errorBlob->Release();

	if (SUCCEEDED(hr))
		return true;
	else
		return false;
}

bool Shader::BuildShaders(const wchar_t* wPath)
{
	HRESULT hr = S_FALSE;

	hr = D3DReadFileToBlob(wPath, &m_ShaderBlob); 
	
	if (SUCCEEDED(hr))
		return true;
	else 
		return false;
}

bool Shader::CreateShader()
{
	ID3D11ShaderReflection* reflector = nullptr;
	ID3D11Device* device = GraphicsEngine::Instance()->GetDevice();
	
	HRESULT hr;
	hr = D3DReflect(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflector);
	
	if(reflector == nullptr)
		return false;

	D3D11_SHADER_DESC shaderDesc;
	reflector->GetDesc(&shaderDesc);

	if (!CreateShaderBuffer(device, &shaderDesc))
	{
		reflector->Release();
		return false;
	}
	if (!CreateInputLayout(device, reflector, &shaderDesc))
	{
		reflector->Release();
		return false;
	}
	if (!CreateConstBuffer(device, reflector, &shaderDesc))
	{
		reflector->Release();
		return false;
	}
	if (!CreateBoundResources(device, reflector, &shaderDesc))
	{
		reflector->Release();
		return false;
	}

	reflector->Release();

	return true;
}

bool Shader::CreateInputLayout(ID3D11Device* device, ID3D11ShaderReflection* reflector, D3D11_SHADER_DESC* shaderDesc)
{
	// Create InputLayout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	int idx = 0;
	for (int idx = 0; idx < (int)shaderDesc->InputParameters; idx++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		reflector->GetInputParameterDesc(idx, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC inputDesc;
		inputDesc.SemanticName = paramDesc.SemanticName;
		inputDesc.SemanticIndex = paramDesc.SemanticIndex;
		inputDesc.InputSlot = 0;
		if (idx == 0)
		{
			inputDesc.AlignedByteOffset = 0;
		}
		else
		{
			inputDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		}
		inputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputDesc.InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		inputLayoutDesc.push_back(inputDesc);
	}

	if(SUCCEEDED(device->CreateInputLayout(inputLayoutDesc.data(), 
	(UINT)inputLayoutDesc.size(), m_ShaderBlob->GetBufferPointer(), 
	m_ShaderBlob->GetBufferSize(), &m_InputLayout)))
		return true;
	else
		return false;
}

bool Shader::CreateConstBuffer(ID3D11Device* device, ID3D11ShaderReflection* reflector, D3D11_SHADER_DESC* shaderDesc)
{
	// Constant Buffer Reflection
	for (unsigned int cbindex = 0; cbindex < shaderDesc->ConstantBuffers; cbindex++)
	{
		ID3D11ShaderReflectionConstantBuffer* cBuffer = reflector->GetConstantBufferByIndex(cbindex);
		D3D11_SHADER_BUFFER_DESC bufferDesc;

		if (SUCCEEDED(cBuffer->GetDesc(&bufferDesc)))
		{
			std::string name{ bufferDesc.Name };
			if (name == "$Globals" || name == "cbPerFrame")
			{
				continue;
			}

			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			reflector->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

			//ConstBuffer* newConstBuffer = new ConstBuffer((const char*)bindDesc.Name, bufferDesc.Size, bindDesc.BindPoint, bindDesc.BindCount);

			//m_ConstBufferList.push_back(shared_ptr<ConstBuffer>(newConstBuffer));
			m_ConstBufferList.push_back(ConstBufferManager::Instance()->CreateConstBuffer((const char*)bindDesc.Name, bufferDesc.Size, bindDesc.BindPoint, bindDesc.BindCount));
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Shader::CreateBoundResources(ID3D11Device* device, ID3D11ShaderReflection* reflector, D3D11_SHADER_DESC* shaderDesc)
{
	/// Shader Resource Reflection
	for (unsigned int rsindex = 0; rsindex < shaderDesc->BoundResources; rsindex++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		if (SUCCEEDED(reflector->GetResourceBindingDesc(rsindex, &bindDesc)))
		{
			switch (bindDesc.Type)
			{
				case D3D_SIT_TEXTURE:
				{
					// Constant Texture Resource
					ShaderResourceBuffer* srvBuffer = new ShaderResourceBuffer(bindDesc.Name, bindDesc.BindPoint, bindDesc.BindCount);

					m_ShaderResourceList.push_back(shared_ptr<ShaderResourceBuffer>(srvBuffer));
					break;
				}
				break;
				case D3D_SIT_SAMPLER:
				{
					// Constant Sampler Resource
					SamplerBuffer* samplerBuffer = new SamplerBuffer(bindDesc.Name, bindDesc.BindPoint, bindDesc.BindCount);
				
					m_SamplerList.push_back(shared_ptr<SamplerBuffer>(samplerBuffer));
					break;
				}
				break;
				case D3D_SIT_UAV_RWTYPED:
				{
					break;
				}
				default:
					break;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Shader::CreateShaderBuffer(ID3D11Device* device, D3D11_SHADER_DESC* shaderDesc)
{
	HRESULT hr;

	m_ShaderType = (ShaderType)D3D11_SHVER_GET_TYPE(shaderDesc->Version);

	// Shader Type에 따라 쉐이더 생성
	switch (m_ShaderType)
	{
		case ShaderType::Pixel:
		{
			auto shaderBuffer = new PixelShaderBuffer;

			hr = device->CreatePixelShader(
				m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(),
				nullptr,
				shaderBuffer->GetShaderRef());

#ifdef _DEBUG
			if (shaderBuffer->GetShaderBuffer() != nullptr)
			{
				DX11SetObjectName(static_cast<ID3D11DeviceChild*>(shaderBuffer->GetShaderBuffer()), GetName());
			}
#endif // _DEBUG

			m_ShaderBuffer = shared_ptr<ShaderBufferBase>(shaderBuffer);
			break;
		}
		case ShaderType::Vertex:
		{
			auto shaderBuffer = new VertexShaderBuffer;

			hr = device->CreateVertexShader(
				m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(),
				nullptr,
				shaderBuffer->GetShaderRef());

#ifdef _DEBUG
			if (shaderBuffer->GetShaderBuffer() != nullptr)
			{
				DX11SetObjectName(static_cast<ID3D11DeviceChild*>(shaderBuffer->GetShaderBuffer()), GetName());
			}
#endif // _DEBUG

			m_ShaderBuffer = shared_ptr<ShaderBufferBase>(shaderBuffer);;
			break;
		}
		case ShaderType::Geometry:
		{
			auto shaderBuffer = new GeometryShaderBuffer;

			hr = device->CreateGeometryShader(
				m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(),
				nullptr,
				shaderBuffer->GetShaderRef());

#ifdef _DEBUG
			if (shaderBuffer->GetShaderBuffer() != nullptr)
			{
				DX11SetObjectName(static_cast<ID3D11DeviceChild*>(shaderBuffer->GetShaderBuffer()), GetName());
			}
#endif // _DEBUG

			m_ShaderBuffer = shared_ptr<ShaderBufferBase>(shaderBuffer);;
			break;
		}
		case ShaderType::Hull:
		{
			auto shaderBuffer = new HullShaderBuffer;

			hr = device->CreateHullShader(
				m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(),
				nullptr,
				shaderBuffer->GetShaderRef());

#ifdef _DEBUG
			if (shaderBuffer->GetShaderBuffer() != nullptr)
			{
				DX11SetObjectName(static_cast<ID3D11DeviceChild*>(shaderBuffer->GetShaderBuffer()), GetName());
			}
#endif // _DEBUG

			m_ShaderBuffer = shared_ptr<ShaderBufferBase>(shaderBuffer);;
			break;
		}
		case ShaderType::Domain:
		{
			auto shaderBuffer = new DomainShaderBuffer;

			hr = device->CreateDomainShader(
				m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(),
				nullptr,
				shaderBuffer->GetShaderRef());

#ifdef _DEBUG
			if (shaderBuffer->GetShaderBuffer() != nullptr)
			{
				DX11SetObjectName(static_cast<ID3D11DeviceChild*>(shaderBuffer->GetShaderBuffer()), GetName());
			}
#endif // _DEBUG

			m_ShaderBuffer = shared_ptr<ShaderBufferBase>(shaderBuffer);;
			break;
		}
		case ShaderType::Compute:
		{
			auto shaderBuffer = new ComputeShaderBuffer;

			hr = device->CreateComputeShader(
				m_ShaderBlob->GetBufferPointer(),
				m_ShaderBlob->GetBufferSize(),
				nullptr,
				shaderBuffer->GetShaderRef());

#ifdef _DEBUG
			if (shaderBuffer->GetShaderBuffer() != nullptr)
			{
				DX11SetObjectName(static_cast<ID3D11DeviceChild*>(shaderBuffer->GetShaderBuffer()), GetName());
			}
#endif // _DEBUG

			m_ShaderBuffer = shared_ptr<ShaderBufferBase>(shaderBuffer);;
			break;
		}
		case ShaderType::Reserved0:
		{
			break;
		}
		default:
			break;
	}

	if(SUCCEEDED(hr))
		return true;
	else
		return false;
}

void Shader::Bind(ID3D11DeviceContext* deviceContext)
{
	m_ShaderBuffer->Bind(deviceContext);
}

//void Shader::ReleaseC()
//{
//	ReleaseCOM(m_ShaderBlob);
//
//	if (m_ShaderBuffer != nullptr)
//	{
//		m_ShaderBuffer->Release();
//		delete m_ShaderBuffer;
//		m_ShaderBuffer = nullptr;
//	}
//
//	ReleaseCOM(m_InputLayout);
//
//	for (auto& cb : m_ConstBufferList)
//	{
//		if (cb != nullptr)
//		{
//			cb->Release();
//			delete cb;
//			cb = nullptr;
//		}
//	}
//
//	for (auto& srv : m_ShaderResourceList)
//	{
//		if (srv != nullptr)
//		{
//			srv->Release();
//			delete srv;
//			srv = nullptr;
//		}
//	}
//
//	for (auto& sampler : m_SamplerList)
//	{
//		if (sampler != nullptr)
//		{
//			sampler->Release();
//			delete sampler;
//			sampler = nullptr;
//		}
//	}
//}
