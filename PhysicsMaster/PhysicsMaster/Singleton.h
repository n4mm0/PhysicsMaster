#pragma once

#define IS_SINGLETON(T)public: static void Init(){ if (T::IsNotInit()){ new T(); } };private: T(){}; T(const T& other); T& operator=(const T& other);

template <typename T> class Singleton
{
	static T* ms_singleton;

public:
	Singleton()
	{
		ms_singleton = static_cast <T*> (this);
	}
	static bool IsNotInit()
	{
		return ms_singleton == nullptr;
	}
	~Singleton()
	{
		delete(ms_singleton);
	}

	static T& GetSingleton()
	{
		return *ms_singleton;
	}

	static void Release()
	{
		delete(ms_singleton);
		ms_singleton = nullptr;
	}

};

template <typename T> T* Singleton <T>::ms_singleton = 0;
