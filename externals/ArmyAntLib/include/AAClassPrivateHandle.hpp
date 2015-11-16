#ifndef CLASS_PRIVATE_HANDLE_HPP_2015_11_13
#define CLASS_PRIVATE_HANDLE_HPP_2015_11_13

#include "AADefine.h"
#include <map>

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

	T_Handle GetHandle(T_Out* src);
	void ReleaseHandle(T_Handle handle);

	T_Out* GetSourceByHandle(T_Handle handle);
	T_In* GetDataByHandle(T_Handle handle);

	T_In* operator[](T_Handle handle);

	std::map<T_Handle, T_Out*> handleRefMap;
	std::map<T_Handle, T_In*> handleMap;

	AA_FORBID_COPY_CTOR(ClassPrivateHandleManager)
	AA_FORBID_EQUAL_OPR(ClassPrivateHandleManager)
};

/******************************** Source Code *********************************/

template <class T_Out, class T_In, class T_Handle>
T_Handle ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::GetHandle(T_Out* src)
{
	auto len = handleMap.size();
	for(T_Handle n = 0; n < len; n++)
	{
		if(handleMap.find(n) == handleMap.end())
		{
			handleMap.insert(std::pair<T_Handle, T_In*>(n, new T_In()));
			handleRefMap.insert(std::pair<T_Handle, T_Out*>(n, src));
			return n;
		}
	}
	handleMap.insert(std::pair<T_Handle, T_In*>(len, new T_In()));
	handleRefMap.insert(std::pair<T_Handle, T_Out*>(len, src));
	return len;
}

template <class T_Out, class T_In, class T_Handle>
void ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::ReleaseHandle(T_Handle handle)
{
	auto ret = handleMap.find(handle);
	if(ret != handleMap.end())
	{
		AA_SAFE_DEL(ret->second);
		handleMap.erase(ret);
	}
}

template <class T_Out, class T_In, class T_Handle>
T_Out* ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::GetSourceByHandle(T_Handle handle)
{
	return handleRefMap.find(handle)->second;
}

template <class T_Out, class T_In, class T_Handle>
T_In* ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::GetDataByHandle(T_Handle handle)
{
	return handleMap.find(handle)->second;
}

template <class T_Out, class T_In, class T_Handle>
T_In* ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::operator[](T_Handle handle)
{
	return GetDataByHandle(handle);
}

} // namespace ArmyAnt

#endif