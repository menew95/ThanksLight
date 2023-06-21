#pragma once

#include <string>
#include <unordered_map>
#include <typeinfo>

// hpp 방법을 찾아보자
#include "ConstBufferDataInfo.h"

namespace TLGraphicsEngine
{
	struct CBBase;
	class ConstBuffer;

	/// <summary>
	/// ConstBuffer에 사용될 CBBase 구조체 컨테이너
	/// </summary>

	class ConstBufferManager
	{
	private:
		ConstBufferManager() {};
	
	public:
		~ConstBufferManager() {};
		
	private:
		static ConstBufferManager* m_Instance;

		std::unordered_map<std::string, CBBase*> m_CBs;

		std::unordered_map<std::string, std::shared_ptr<ConstBuffer>> m_constBufferMap;

	public:
		void Init();
		static ConstBufferManager* Instance();
		static void Finalize();
		void Release();

		template<class T>
		CBBase* GetCB(T* data);

		template<class T>
		void RegistType(CBBase* cb);

		std::shared_ptr<ConstBuffer> CreateConstBuffer(const char* name, int size, int bindPoint, int bindCnt);
		std::shared_ptr<ConstBuffer> GetConstBuffer(const char* name);
	};

	template<class T>
	void ConstBufferManager::RegistType(CBBase* cb)
	{
		auto typeinfo = std::string(typeid(T).name());
		auto cbbase = m_CBs.find(typeinfo);

		if (cbbase == m_CBs.end())
		{
			m_CBs.insert(make_pair(typeinfo, cb));
		}
	}

	template<class T>
	CBBase* ConstBufferManager::GetCB(T* data)
	{
		auto cb = m_CBs.find(std::string(typeid(T).name()));

		if (cb != m_CBs.end())
		{
			memcpy(cb->second, data, sizeof(*data));
			return cb->second;
		}

		return nullptr;
	}
}