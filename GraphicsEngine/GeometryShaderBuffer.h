#pragma once
#include "ShaderBufferBase.h"

struct ID3D11GeometryShader;

namespace TLGraphicsEngine
{
	/// <summary>
	/// Geometry Shader Buffer
	/// </summary>

	class GeometryShaderBuffer :
	    public ShaderBufferBase
	{
	public:
		GeometryShaderBuffer();
		~GeometryShaderBuffer();

	public:
		void* GetShader();
		void Bind(ID3D11DeviceContext* deviceContext) override;
		
		ID3D11GeometryShader** GetShaderRef() { return m_geometryShader.GetAddressOf(); }
		ID3D11GeometryShader* GetShaderBuffer() { return m_geometryShader.Get(); }

	private:
		ComPtr<ID3D11GeometryShader> m_geometryShader;
	};
}