/*  
 * Copyright (c) 2015 ArmyAnt
 * 版权所有 (c) 2015 ArmyAnt
 *
 * Licensed under the BSD License, Version 2.0 (the License); 
 * 本软件使用BSD协议保护, 协议版本:2.0
 * you may not use this file except in compliance with the License. 
 * 使用本开源代码文件的内容, 视为同意协议
 * You can read the license content in the file "ARMYANT.COPYRIGHT.BSD_LICENSE.MD" at the root of this project
 * 您可以在本项目的根目录找到名为"ARMYANT.COPYRIGHT.BSD_LICENSE.MD"的文件, 来阅读协议内容
 * You may also obtain a copy of the License at 
 * 您也可以在此处获得协议的副本:
 * 
 *     http://opensource.org/licenses/BSD-3-Clause
 * 
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an AS IS BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * 除非法律要求或者版权所有者书面同意,本软件在本协议基础上的发布没有任何形式的条件和担保,无论明示的或默许的.
 * See the License for the specific language governing permissions and limitations under the License. 
 * 请在特定限制或语言管理权限下阅读协议
 */

#ifndef CLASS_PRIVATE_HANDLE_HPP_2015_11_13
#define CLASS_PRIVATE_HANDLE_HPP_2015_11_13

#include "AATripleMap.hpp"

namespace ArmyAnt {

//此类用于保护性隐藏C++类中的私有成员，将私有成员定义在cpp实现文件的另一个类中（public），而原类增加一个 const T_Handle handle 公有成员
//实现文件定义一个static本类实例，作为私有成员管理器，原类的函数体内通过handle和管理器来获取私有成员
//T_Out参数代表原类型，T_In参数代表包含私有成员的类型，T_Handle代表句柄的类型，可从任意整型中选择，默认为uint32
//使用时，不要把管理器实例或者T_In类的任何信息，暴露在cpp文件之外

template <class T_Out, class T_In, class T_Handle = uint32>
class ClassPrivateHandleManager
{
public:
	ClassPrivateHandleManager() {}
	~ClassPrivateHandleManager() {}

	//创建一个内部类实例，这通常是在建立外部实例时进行调用的
	T_Handle GetHandle(T_Out* src, T_In* newObject = new T_In());
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
T_Handle ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::GetHandle(T_Out* src, T_In* newObject)
{
	auto len = handleMap.Size();
	//获取当前未使用的最小句柄号
	for(T_Handle n = 0; n < len; n++)
	{
		//设置句柄，创建内部实例，并关联到外部实例
		if(handleMap.Find(n) == handleMap.End())
		{
			handleMap.Insert(n, src, newObject);
			return n;
		}
	}
	//没有未使用的中间句柄号，则在结尾添加新最大句柄号，并创建内部实例、关联到外部实例
	handleMap.Insert(len, src, newObject);
	return len;
}

template <class T_Out, class T_In, class T_Handle>
void ArmyAnt::ClassPrivateHandleManager<T_Out, T_In, T_Handle>::ReleaseHandle(T_Handle handle)
{
	Iterator_TripleMap<T_Handle, T_Out*, T_In*> ret = handleMap.Find(handle);
	//销毁内部实例，解除关联
	if(ret != handleMap.End())
	{
		delete ret->third;
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