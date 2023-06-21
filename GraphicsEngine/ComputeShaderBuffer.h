#pragma once
#include "ShaderBufferBase.h"

struct ID3D11ComputeShader;

namespace TLGraphicsEngine
{
	/// <summary>
	/// Comput Shader Buffer
	/// </summary>

	class ComputeShaderBuffer :
	    public ShaderBufferBase
	{
	public:
		ComputeShaderBuffer();
		~ComputeShaderBuffer();

	public:
		void Release();

		void* GetShader();
		void Bind(ID3D11DeviceContext* deviceContext) override;
		ID3D11ComputeShader** GetShaderRef() { return m_computeShaderBuffer.GetAddressOf(); }
		ID3D11ComputeShader* GetShaderBuffer() { return m_computeShaderBuffer.Get(); }

	private:
		ComPtr<ID3D11ComputeShader> m_computeShaderBuffer;
	};
}