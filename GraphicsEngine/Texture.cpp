#include "pch.h"
#include "Texture.h"
#include "GraphicsEngine.h"
#include "DDSTextureLoader.h"

#include "GraphicResourceManager.h"

#include "../DXTK/Inc/WICTextureLoader.h"

#ifdef _DEBUG
#ifndef x64
#pragma comment(lib, "DirectXTK.lib")
#else
#pragma comment(lib, "DirectXTKx64Debug.lib")
#endif // WIN32
#else
#ifndef x64
#pragma comment(lib, "DirectXTKr.lib")
#else
#pragma comment(lib, "DirectXTKx64Release.lib")
#endif // WIN32
#endif // _DEBUG


TLGraphicsEngine::Texture::Texture(const char* name) :
	ITexture(name)
{

}

TLGraphicsEngine::Texture::~Texture()
{
}

//void TLGraphicsEngine::Texture::Release()
//{
//	ReleaseCOM(m_SRV);
//	GraphicResourceManager::Instance()->ReleaseResource(m_ResourceType, m_Name);
//}
//
//void TLGraphicsEngine::Texture::ReleaseC()
//{
//	ReleaseCOM(m_SRV);
//}

bool TLGraphicsEngine::Texture::MakeTexture(const char* path)
{
	std::string sPath = std::string(path);
	std::wstring wpath(sPath.begin(), sPath.end());
	
	HRESULT hr;

	// Todo : 문자열 검색이 아닌 확장자 검색으로 바꾸도록 해야함
	if (sPath.find(".dds") != -1)
	{
		hr = CreateDDSTextureFromFile(GraphicsEngine::Instance()->GetDevice(),
			wpath.c_str(), m_Resource.GetAddressOf(), m_SRV.GetAddressOf());
	}
	else if(sPath.find(".png") != -1)
	{
		hr = CreateWICTextureFromFile(GraphicsEngine::Instance()->GetDevice(),
			wpath.c_str(), m_Resource.GetAddressOf(), m_SRV.GetAddressOf());
	}
	else if (sPath.find(".jpeg") != -1)
	{
		hr = CreateWICTextureFromFile(GraphicsEngine::Instance()->GetDevice(),
			wpath.c_str(), m_Resource.GetAddressOf(), m_SRV.GetAddressOf());
	}
	else if (sPath.find(".bmp") != -1)
	{
		hr = CreateWICTextureFromFile(GraphicsEngine::Instance()->GetDevice(),
			wpath.c_str(), m_Resource.GetAddressOf(), m_SRV.GetAddressOf());
	}
	else if (sPath.find(".gif") != -1)
	{
		hr = CreateWICTextureFromFile(GraphicsEngine::Instance()->GetDevice(),
			wpath.c_str(), m_Resource.GetAddressOf(), m_SRV.GetAddressOf());
	}

	if (SUCCEEDED(hr))
	{
#ifdef _DEBUG
		/*if (m_Resource)
		{
			DX11SetObjectName(m_Resource.Get(), GetName());

			if (m_SRV)
				DX11SetObjectNameSubscript(m_SRV.Get(), GetName(), ".SRV");
		}*/

#endif // _DEBUG
		return true;
	}
	else
		return false;
}
