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
	ID3D11Resource* texResource = nullptr;

	HRESULT hr;

	// Todo : ���ڿ� �˻��� �ƴ� Ȯ���� �˻����� �ٲٵ��� �ؾ���
	if (sPath.find(".dds") != -1)
	{
		hr = CreateDDSTextureFromFile(GraphicsEngine::Instance()->GetDevice(),
			wpath.c_str(), &texResource, m_SRV.GetAddressOf());
	}
	else if(sPath.find(".png") != -1)
	{
		hr = CreateWICTextureFromFile(GraphicsEngine::Instance()->GetDevice(),
			wpath.c_str(), &texResource, m_SRV.GetAddressOf());
	}
	else if (sPath.find(".jpeg") != -1)
	{
		hr = CreateWICTextureFromFile(GraphicsEngine::Instance()->GetDevice(),
			wpath.c_str(), &texResource, m_SRV.GetAddressOf());
	}
	else if (sPath.find(".bmp") != -1)
	{
		hr = CreateWICTextureFromFile(GraphicsEngine::Instance()->GetDevice(),
			wpath.c_str(), &texResource, m_SRV.GetAddressOf());
	}
	else if (sPath.find(".gif") != -1)
	{
		hr = CreateWICTextureFromFile(GraphicsEngine::Instance()->GetDevice(),
			wpath.c_str(), &texResource, m_SRV.GetAddressOf());
	}

	texResource->Release(); 

	if (SUCCEEDED(hr))
	{
#ifdef _DEBUG
		m_SRV->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(GetName()) - 1, GetName());
#endif // _DEBUG
		return true;
	}
	else
		return false;
}