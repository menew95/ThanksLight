#pragma once

#include "ShaderBufferBase.h"

struct ID3D11VertexShader;

namespace TLGraphicsEngine
{
	/// <summary>
	/// Vertex Shader Buffer
	/// </summary>
	
	class VertexShaderBuffer :
	    public ShaderBufferBase
	{
	public:
		VertexShaderBuffer();
		~VertexShaderBuffer();

	public:

		void* GetShader();
		void Bind(ID3D11DeviceContext* deviceContext) override;
		
		ID3D11VertexShader** GetShaderRef() { return m_vertexShaderBuffer.GetAddressOf(); }
		ID3D11VertexShader* GetShaderBuffer() { return m_vertexShaderBuffer.Get(); }

	private:
		ComPtr<ID3D11VertexShader> m_vertexShaderBuffer;
	};
}