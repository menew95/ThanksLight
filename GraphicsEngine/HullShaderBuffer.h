#pragma once
#include "ShaderBufferBase.h"

struct  ID3D11HullShader;

namespace TLGraphicsEngine
{
	/// <summary>
	/// Hull Shader Buffer
	/// </summary>
	
	class HullShaderBuffer :
	    public ShaderBufferBase
	{
	public:
		HullShaderBuffer();
		~HullShaderBuffer();

	public:
		void* GetShader();
		void Bind(ID3D11DeviceContext* deviceContext) override;
		
		ID3D11HullShader** GetShaderRef() { return m_hullShader.GetAddressOf(); }
		ID3D11HullShader* GetShaderBuffer() { return m_hullShader.Get(); }

	private:
		ComPtr<ID3D11HullShader> m_hullShader;
	};
}