#pragma once

//#include "../Common/ResourceBase.h"
#include "Export/ResourceBase.h"
#include "ShaderBufferBase.h"

struct ID3DX11Effect;
struct ID3D11InputLayout;

struct ID3D11Device;
struct ID3D11ShaderReflection;
typedef struct _D3D11_SHADER_DESC D3D11_SHADER_DESC;

namespace TLGraphicsEngine
{
	struct CBBase;
	class ConstBuffer;
	class ShaderResourceBuffer;
	class SamplerBuffer;

	enum class VertexType
	{
		Default = 0,
		Skin = 1,
		PosUV = 2,
		PosNormalUV = 3
	};

	enum class ShaderType
	{
		Pixel = 0,
		Vertex = 1,
		Geometry = 2,
		Hull = 3,
		Domain = 4,
		Compute = 5,
		Reserved0 = 0xFFF0
	};

	class Shader : 
		public ResourceBase
	{
	public:
		Shader() {}
		Shader(const char* name);
		Shader(const Shader&) = default;
		Shader(Shader&&) = default;
		virtual ~Shader();

		//void Release();
		//void ReleaseC();

	private:
		ShaderType m_ShaderType = ShaderType::Vertex;

		ComPtr<ID3DBlob> m_ShaderBlob = nullptr;
		ComPtr<ID3D11InputLayout> m_InputLayout = nullptr;

		shared_ptr<ShaderBufferBase> m_ShaderBuffer = nullptr;


		std::vector<shared_ptr<ConstBuffer>> m_ConstBufferList;
		std::vector<shared_ptr<SamplerBuffer>> m_SamplerList;
		std::vector<shared_ptr<ShaderResourceBuffer>> m_ShaderResourceList;

	private:
		bool CreateShaderBuffer(ID3D11Device* device, D3D11_SHADER_DESC* shaderDesc);
		bool CreateInputLayout(ID3D11Device* device, ID3D11ShaderReflection* reflector, D3D11_SHADER_DESC* shaderDesc);
		bool CreateConstBuffer(ID3D11Device* device, ID3D11ShaderReflection* reflector, D3D11_SHADER_DESC* shaderDesc);
		bool CreateBoundResources(ID3D11Device* device, ID3D11ShaderReflection* reflector, D3D11_SHADER_DESC* shaderDesc);

	public:
		bool CreateShader();
		bool Update(CBBase** const cbs, ID3D11ShaderResourceView*** srvs);
		bool Update(CBBase** const cbs, std::vector<ID3D11ShaderResourceView**>& srvs);
		bool Update(std::vector<CBBase*>& cbs, std::vector<ID3D11ShaderResourceView**>& srvs);

		ID3D11InputLayout* GetInputLayout() { return m_InputLayout.Get(); };
		
		template<class ShaderTy>
		ShaderTy* GetShader();

		bool ComfileBuffer(const wchar_t* wPath,/* const D3D_SHADER_MACRO* pDefines,*/ LPCSTR entry_point, LPCSTR shader_model);
		bool BuildShaders(const wchar_t* wPath);

		void Bind(ID3D11DeviceContext* deviceContext);
	};

	template<class ShaderTy>
	ShaderTy* Shader::GetShader()
	{
		return reinterpret_cast<ShaderTy*>(m_ShaderBuffer->GetShader());
	}
}