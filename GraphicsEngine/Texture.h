#pragma once

#include "Export/ITexture.h"

struct ID3D11ShaderResourceView;

namespace TLGraphicsEngine
{
	class Texture : public ITexture
	{
	public:
		Texture(const char* name);
		//Texture(const std::wstring& path);
		//Texture(const std::string& path);
		Texture(const Texture&) = default;
		Texture(Texture&&) = default;
		~Texture();

	private:
		ComPtr<ID3D11Resource> m_Resource;
		ComPtr<ID3D11ShaderResourceView> m_SRV;

	public:
		bool MakeTexture(const char* path);

		void SetSRV(ID3D11ShaderResourceView* srv) { m_SRV = srv; }
		ID3D11ShaderResourceView* GetSRV() { return m_SRV.Get(); }
		ID3D11ShaderResourceView** GetSRVR() { return m_SRV.GetAddressOf(); }
	};
}