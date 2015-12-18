#ifndef CLASS_PRIVATE_HANDLE_HPP_2015_11_13
#define CLASS_PRIVATE_HANDLE_HPP_2015_11_13

#include "AADefine.h"
#include "AATripleMap.hpp"

namespace ArmyAnt {

//此类用于保护性隐藏C++类中的私有成员，将私有成员定义在cpp实现文件的另一个类中（public），而原类增加一个 const T_Handle handle 公有成员
//实现文件定义一个static本类实例，作为私有成员管理器，原类的函数体内通过handle和管理器来获取私有成员
//T_Out参数代表原类型，T_In参数代表包含私有成员的类型，T_Handle代表句柄的类型，可从任意整型中选择，默认为DWORD
//使用时，不要把管理器实例或者T_In类的任何信息，暴露在cpp文件之外

template <class T_Out, class T_In, class T_Handle = DWORD>
class ClassPrivateHandleManager
{
public:
	ClassPrivateHandleManager() {}
	~ClassPrivateHandleManager() {}

	//创建一个内部类实例，这通常是在建立外部实例时进行调用的
	T_Handle GetHandle(T_Out* src);
	//销毁内部实例，这通常是与外部实例的析构一起进行的
	void ReleaseHandle(T_Handle handle);

	//根据句柄获取外部实例，这通常是用于对C接口时的调用，C语言使用句柄
	T_Out* GetSourceByHandle(T_Handle handle);
	//根据句柄获取内部实例，所有外部对象的公有函数，都需要调用此函数才能访问内部数据
	T_In* GetDataByHandle(T_Handle handle);

	//根据句柄获取内部实例，是GetDataByHandle的快捷调用法
	T_In* operator[](T_Handle handle);

	//内外实例以及句柄的表图
	TripleMap<T_Handle, T_Out*, T_In*> handleMap;


	AA_FORBID_COPY_CTOR(ClassPrivateHandleManager);
	AA_FORBID_ASSGN_OPR(ClassPrivateHandleManager);
};

/******************************** Source Code *********************************/

template <class T_Out, class T_In, class T_Handle>
T_Handle ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::GetHandle(T_Out* src)
{
	auto len = handleMap.Size();
	//获取当前未使用的最小句柄号
	for(T_Handle n = 0; n < len; n++)
	{
		//设置句柄，创建内部实例，并关联到外部实例
		if(handleMap.Find(n) == handleMap.End())
		{
			handleMap.Insert(n, src, new T_In());
			return n;
		}
	}
	//没有未使用的中间句柄号，则在结尾添加新最大句柄号，并创建内部实例、关联到外部实例
	handleMap.Insert(len, src, new T_In());
	return len;
}

template <class T_Out, class T_In, class T_Handle>
void ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::ReleaseHandle(T_Handle handle)
{
	auto ret = handleMap.Find(handle);
	//销毁内部实例，解除关联
	if(ret != handleMap.End())
	{
		AA_SAFE_DEL(ret->third);
		handleMap.Erase(handle);
	}
}

template <class T_Out, class T_In, class T_Handle>
T_Out* ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::GetSourceByHandle(T_Handle handle)
{
	auto ret = handleMap.Find(handle);
	return ret == handleMap.End() ? nullptr : ret->second;
}

template <class T_Out, class T_In, class T_Handle>
T_In* ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::GetDataByHandle(T_Handle handle)
{
	auto ret = handleMap.Find(handle);
	return ret == handleMap.End() ? nullptr : ret->third;
}

template <class T_Out, class T_In, class T_Handle>
T_In* ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::operator[](T_Handle handle)
{
	return GetDataByHandle(handle);
}

} // namespace ArmyAnt

#endif