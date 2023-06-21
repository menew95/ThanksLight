#include "pch.h"
#include "ConstBufferManager.h"
#include "ConstBufferDataInfo.h"
#include "ConstBuffer.h"

TLGraphicsEngine::ConstBufferManager* TLGraphicsEngine::ConstBufferManager::m_Instance = nullptr;

#define RegistCB(Class)\
m_Instance->RegistType<Class>(new Class);

#define RegistCBList()\
RegistCB(VANTA_PBR_PSCB)\
RegistCB(PostProcess_Blur_PSCB)\
RegistCB(PostProcess_Up_PSCB)\
RegistCB(PostProcess_Bloom_PSCB)\
RegistCB(PostProcess_ToneMapping_PSCB)\
RegistCB(Light_CB)\
RegistCB(PerFrame_CB)\
RegistCB(PerObject_CB)
//RegistCB(Skin_VSCB)\
//RegistCB(Mesh_VSCB)\
//RegistCB(Light_VSCB)\
//RegistCB(Light_PSCB)\
//RegistCB(SSAO_CB)\
//RegistCB(PBR_PSCB)\
//RegistCB(Shadow_Skin_Mesh_VSCB)\
//RegistCB(Shadow_Static_Mesh_VSCB)


void TLGraphicsEngine::ConstBufferManager::Init()
{
	RegistCBList()

	CreateConstBuffer("cbPerFrame", sizeof(PerFrame_CB), 0, 1);
}

TLGraphicsEngine::ConstBufferManager* TLGraphicsEngine::ConstBufferManager::Instance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new ConstBufferManager;
		m_Instance->Init();
	}
	return m_Instance;
}

void TLGraphicsEngine::ConstBufferManager::Finalize()
{
	if (m_Instance != nullptr)
	{
		m_Instance->Release();
		delete m_Instance;
	}
}

void TLGraphicsEngine::ConstBufferManager::Release()
{
	for (auto cb : m_CBs)
	{
		if (cb.second != nullptr)
		{
			delete cb.second;
			cb.second = nullptr;
		}
	}
	m_CBs.clear();
}

shared_ptr<TLGraphicsEngine::ConstBuffer> TLGraphicsEngine::ConstBufferManager::CreateConstBuffer(const char* name, int size, int bindPoint, int bindCnt)
{
	auto _find = m_constBufferMap.find(name);

	// 이미 만들어진 컨스트 버퍼가 아니면 생성;
	if (_find != m_constBufferMap.end())
	{
		return _find->second;
	}

	shared_ptr<ConstBuffer> _newConstBuffer = make_shared<ConstBuffer>(name, size, bindPoint, bindCnt);

	m_constBufferMap.insert(make_pair(name, _newConstBuffer));

	return _newConstBuffer;
}

std::shared_ptr<TLGraphicsEngine::ConstBuffer> TLGraphicsEngine::ConstBufferManager::GetConstBuffer(const char* name)
{
	auto _find = m_constBufferMap.find(name);

	if (_find != m_constBufferMap.end())
	{
		return _find->second;
	}

	return nullptr;
}
