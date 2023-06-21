#pragma once

#include "ShaderBufferBase.h"

struct ID3D11PixelShader;

namespace TLGraphicsEngine
{

	/// <summary>
	/// Pixel Shader Buffer
	/// </summary>

	class PixelShaderBuffer :
	    public ShaderBufferBase
	{
	public:
		PixelShaderBuffer();
		~PixelShaderBuffer();

	public:
		void* GetShader();
		void Bind(ID3D11DeviceContext* deviceContext) override;
		
		ID3D11PixelShader** GetShaderRef() { return m_pixelShaderBuffer.GetAddressOf(); }
		ID3D11PixelShader* GetShaderBuffer() { return m_pixelShaderBuffer.Get(); }

	private:
		ComPtr<ID3D11PixelShader> m_pixelShaderBuffer = nullptr;
	};
}