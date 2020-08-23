#pragma once
#include <iostream>

template <typename T, typename ... Args>
T* __new(Args&&...args)
{
	return new T(args...);
}

#define OBJECT_NEW(T, ...) __new<T>(__VA_ARGS__)

// 内存分配模式
struct pool_mode {};	// 内存池模式
struct system_mode {};	// 系统模式

#define DEFAULT_MODE pool_mode	// 默认内存分配模式
#define DEFAULT_FACTORY_HOLDER FactoryHolder<DEFAULT_MODE> // 默认内存分配模式的工厂指挥者

// 内存池，当然这个是冒牌的内存池，本质还是系统分配内存，为了实现了接口而做。
template <typename T>
class Pool
{
public:
	void* allocate() { return malloc(sizeof(T)); }
};

class SignalItemData
{
public:
	SignalItemData() {}
	SignalItemData(int) {}
	void setPool(Pool<SignalItemData> *pool) {}
};

typedef double SignalHeadInfo;

class SignalValueInfo
{
public:
	SignalValueInfo() {}
	SignalValueInfo(int) {}
	SignalValueInfo(double, int) {}
};

// 模板工厂
template <class T, class P = system_mode>
class TemplateFactory
{
public:
	// 变长参数，不管你的构造函数的参数个数，参数类型是啥样，都可以创建
	template <typename ... Args>
	T* createProduct(Args&&...args)
	{
		std::cout << "normal mode create" << std::endl;
		return new T(std::forward<Args>(args)...); // 完美转发
	}
};

// 模板工厂偏特化
template <class T>
class TemplateFactory<T, pool_mode>
{
public:
	template <typename ... Args>
	T* createProduct(Args&&...args)
	{
		std::cout << "pool mode create" << std::endl;
		void* addr = m_pool.allocate();
		return new(addr) T(std::forward<Args>(args)...);
	}

private:
	Pool<T> m_pool;
};

// 模板工厂全特化
template <>
class TemplateFactory<SignalItemData, pool_mode>
{
public:
	template <typename ... Args>
	SignalItemData* createProduct(Args&&...args)
	{
		std::cout << "pool mode create signalItemData" << std::endl;
		void* addr = m_pool.allocate();
		auto item = new(addr) SignalItemData(std::forward<Args>(args)...);
		item->setPool(&m_pool);
		return item;
	}

private:
	Pool<SignalItemData> m_pool;
};

/**
 * @brief 工厂指挥者， 模板参数M: 内存分配模式
 **/
template <typename M = system_mode>
class FactoryHolder
{
public:
	template <typename ... Args>
	SignalItemData* createItem(Args&&... args)
	{
		return m_itemFactory.createProduct(std::forward<Args>(args)...);
	}

	template <typename ... Args>
	SignalHeadInfo* createHeader(Args&& ... args)
	{
		return m_headerFactory.createProduct(std::forward<Args>(args)...);
	}

	template <typename ... Args>
	SignalValueInfo* createValue(Args&& ... args)
	{
		return m_valueFactory.createProduct(std::forward<Args>(args)...);
	}

private:
	TemplateFactory<SignalItemData, M> m_itemFactory;
	TemplateFactory<SignalHeadInfo> m_headerFactory;
	TemplateFactory<SignalValueInfo, M> m_valueFactory;
};
