#pragma once
#include <iostream>

template <typename T, typename ... Args>
T* __new(Args&&...args)
{
	return new T(args...);
}

#define OBJECT_NEW(T, ...) __new<T>(__VA_ARGS__)

// �ڴ����ģʽ
struct pool_mode {};	// �ڴ��ģʽ
struct system_mode {};	// ϵͳģʽ

#define DEFAULT_MODE pool_mode	// Ĭ���ڴ����ģʽ
#define DEFAULT_FACTORY_HOLDER FactoryHolder<DEFAULT_MODE> // Ĭ���ڴ����ģʽ�Ĺ���ָ����

// �ڴ�أ���Ȼ�����ð�Ƶ��ڴ�أ����ʻ���ϵͳ�����ڴ棬Ϊ��ʵ���˽ӿڶ�����
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

// ģ�幤��
template <class T, class P = system_mode>
class TemplateFactory
{
public:
	// �䳤������������Ĺ��캯���Ĳ�������������������ɶ���������Դ���
	template <typename ... Args>
	T* createProduct(Args&&...args)
	{
		std::cout << "normal mode create" << std::endl;
		return new T(std::forward<Args>(args)...); // ����ת��
	}
};

// ģ�幤��ƫ�ػ�
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

// ģ�幤��ȫ�ػ�
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
 * @brief ����ָ���ߣ� ģ�����M: �ڴ����ģʽ
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
