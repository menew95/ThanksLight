#pragma once

template <typename T>
class Singleton
{
private:
	static T* m_instance;

public:
	static T& Instance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new T();
		}

		return *m_instance;
	}

	static void Release()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
		}
	}
};

template <typename T>
T* Singleton<T>::m_instance = nullptr;