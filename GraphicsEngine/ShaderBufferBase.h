#pragma once

//#include "../Common/ResourceBase.h"
#include "Export/ResourceBase.h"

struct  ID3D11VertexShader;
struct ID3D11DeviceContext;

namespace TLGraphicsEngine
{
	/// <summary>
	/// Shader Buffer Base
	/// </summary>

	class ShaderBufferBase :
	    public ResourceBase
	{
	public:
		ShaderBufferBase();

	public:
		virtual void* GetShader() abstract;
		virtual void Bind(ID3D11DeviceContext* deviceContext) abstract;
	};
}