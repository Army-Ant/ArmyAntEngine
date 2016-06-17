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

#ifndef AA_FRAGMENT_H_2015_12_31
#define AA_FRAGMENT_H_2015_12_31

/*
 * The base tool functions and datas in the library
 * 本库的一些基础工具和数据.如果是数据, 实现在ArmyAntLib.cpp中
 */

#if defined DEBUG
#include <assert.h>
#elif defined _cplusplus
#include <stdexcept>
#else

#endif

#include <iostream>

#ifdef _cplusplus
#undef max
#undef min
#undef AA_SAFE_DEL
#undef AA_SAFE_DELALL
#endif

#ifdef OS_WINDOWS
#define AA_INTEGER(num, len) num##len
#else
#define AA_INTEGER(num, len) num
#endif

typedef long long int64;
typedef unsigned long long uint64;

#ifdef _CMAKE
typedef long long __int64;
#endif

namespace ArmyAnt {

namespace Constant {

// Mathimatic constants
const double c_natrualBase = 2.7182818284590452353;	// 自然对数底
const double c_pi =	3.1415926535897932384;			// 圆周率
const double c_goldenScale = 0.6180339887498948482;	// 黄金分割比

// Physics constants
const int32 c_lightVacuum = AA_INTEGER(299792458,i32);	// (m/s)	光速
const float c_gravitation = 6.67310e-11f;	// (m^3/(kg*s^2))	引力常数
const double c_planck = 6.6260687652e-34;	// (J*s)	普朗克常量
const double c_elementaryCharge = 1.60217646263e-19;	// (C)	电子电荷量
const int64 c_lightYear = AA_INTEGER(9460730472580800,i64);	// (m)	光年
const double c_electronVolt = 1.60217646263e-19;	// (J)	电子伏
const double c_avogadro = 6.0221422026e23;		// 阿伏伽德罗常量(常数), 摩尔质量系数

// Binary units conversion
const int16 c_2xK = AA_INTEGER(0b10000000000,i16);
const int32 c_2xM = AA_INTEGER(0b100000000000000000000,i32);
const int32 c_2xG = AA_INTEGER(0b1000000000000000000000000000000,i32);
const int64 c_2xT = AA_INTEGER(0b10000000000000000000000000000000000000000,i64);
const int64 c_2xP = AA_INTEGER(0b100000000000000000000000000000000000000000000000000,i64);
const int64 c_2xE = AA_INTEGER(0b1000000000000000000000000000000000000000000000000000000000000,i64);

}

namespace Fragment {

template <class NUM1, class NUM2> 
inline NUM1 min(NUM1 num1, NUM2 num2)
{
	return num1 > num2 ? NUM1(num2) : num1;
}

template <class NUM1, class NUM2> 
inline NUM1 max(NUM1 num1, NUM2 num2)
{
	return num1 < num2 ? NUM1(num2) : num1;
}

template<class PTR>
inline PTR&AA_SAFE_DEL(PTR&ptr)
{
	if(ptr != nullptr)
	{
		delete ptr;
		ptr = nullptr;
	}
	return ptr;
}

template <class PTR>
inline void AA_SAFE_DELALL(PTR&ptr)
{
	if(ptr != nullptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

} // namespace Fragment

} // namespace ArmyAnt

#undef AA_INTEGER

#endif
