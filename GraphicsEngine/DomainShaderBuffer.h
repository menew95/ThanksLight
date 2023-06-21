#pragma once
#include "ShaderBufferBase.h"

struct ID3D11DomainShader;

namespace TLGraphicsEngine
{
	/// <summary>
	/// Domain Shader Buffer
	/// </summary>

	class DomainShaderBuffer :
	    public ShaderBufferBase
	{
	public:
		DomainShaderBuffer();
		~DomainShaderBuffer();

	public:
		void* GetShader();
		void Bind(ID3D11DeviceContext* deviceContext) override;
		ID3D11DomainShader** GetShaderRef() { return m_domainShaderBuffer.GetAddressOf(); }
		ID3D11DomainShader* GetShaderBuffer() { return m_domainShaderBuffer.Get(); }

	private:
		ComPtr<ID3D11DomainShader> m_domainShaderBuffer = nullptr;

	};
}