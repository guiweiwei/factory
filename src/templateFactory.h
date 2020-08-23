#pragma once
#include <iostream>

template <typename T, typename ... Args>
T* __new(Args&&...args)
{
	return new T(args...);
}

#define OBJECT_NEW(T, ...) __new<T>(__VA_ARGS__)

struct pool_mode {};
struct normal_mode {};

#define DEFAULT_MODE normal_mode
#define DEFAULT_FACTORY Factory<DEFAULT_MODE>

typedef int SignalItemData;
typedef double SignalHeadInfo;
class SignalValueInfo
{
public:
	SignalValueInfo() {}
	SignalValueInfo(int) {}
	SignalValueInfo(double, int) {}
};

template <typename T>
class Pool
{
public:
	void* allocate() { return malloc(sizeof(T)); }
};

template <class T, class P = normal_mode>
class templateFactory
{
public:
	template <typename ... Args>
	T* createFactory(Args&&...args)
	{
		std::cout << "normal mode create" << std::endl;
		return new T(args...);
	}
};


template <class T>
class templateFactory<T, pool_mode>
{
public:
	template <typename ... Args>
	T* createFactory(Args&&...args)
	{
		std::cout << "pool mode create" << std::endl;
		void* addr = m_pool.allocate();
		return new(addr) T(args...);
	}

private:
	Pool<T> m_pool;
};

template <typename M>
class Factory
{
public:
	template <typename ... Args>
	SignalItemData* createItem(Args&&... args)
	{
		return m_itemFactory.createFactory(args...);
	}

	template <typename ... Args>
	SignalHeadInfo* createHeader(Args&& ... args)
	{
		return m_headerFactory.createFactory(args...);
	}

	template <typename ... Args>
	SignalValueInfo* createValue(Args&& ... args)
	{
		return m_valueFactory.createFactory(args...);
	}

private:
	templateFactory<SignalItemData, M> m_itemFactory;
	templateFactory<SignalHeadInfo, M> m_headerFactory;
	templateFactory<SignalValueInfo, M> m_valueFactory;
};
