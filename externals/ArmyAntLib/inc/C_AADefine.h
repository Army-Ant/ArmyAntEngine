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

#ifndef C_A_A_DEFINE_H_2015_11_11
#define C_A_A_DEFINE_H_2015_11_11

//#include <yvals.h>

#ifdef OS_WINDOWS
#define AA_INTEGER(num, len) num##len
#else
#define AA_INTEGER(num, len) num
#endif

// 定长整数定义
typedef char int8;
const int8 AA_INT8_MAX = AA_INTEGER(0x7f, i8);
const int8 AA_INT8_MIN = AA_INTEGER(0x80, i8);
typedef unsigned char uint8;
const uint8 AA_UINT8_MAX = AA_INTEGER(0xff, ui8);
typedef short int16;
const int16 AA_INT16_MAX = AA_INTEGER(0x7fff, i16);
const int16 AA_INT16_MIN = AA_INTEGER(0x8000, i16);
typedef unsigned short uint16;
const uint16 AA_UINT16_MAX = AA_INTEGER(0xffff, ui16);
typedef int int32;
const int32 AA_INT32_MAX = AA_INTEGER(0x7fffffff, i32);
const int32 AA_INT32_MIN = AA_INTEGER(0x80000000, i32);
typedef unsigned int uint32;
const uint32 AA_UINT32_MAX = AA_INTEGER(0xffffffff, ui32);

#if defined _64BIT || defined _cplusplus
typedef long long int64;
const int64 AA_INT64_MAX = AA_INTEGER(0x7fffffffffffffff, i64);
const int64 AA_INT64_MIN = AA_INTEGER(0x8000000000000000, i64);
typedef unsigned long long uint64;
const uint64 AA_UINT64_MAX = AA_INTEGER(0xffffffffffffffff, ui64);
#endif

// 变长整数定义
#ifdef _32BIT
typedef int mac_int;
typedef unsigned int mac_uint;
const mac_int AA_MAC_INT_MAX = AA_INT32_MAX;
const mac_int AA_MAC_INT_MIN = AA_INT32_MIN;
const mac_uint AA_MAC_UINT_MAX = AA_UINT32_MAX;
#else
typedef long long mac_int;
typedef unsigned long long mac_uint;
const mac_int AA_MAC_INT_MAX = AA_INT64_MAX;
const mac_int AA_MAC_INT_MIN = AA_INT64_MIN;
const mac_uint AA_MAC_UINT_MAX = AA_UINT64_MAX;
#endif

#undef AA_INTEGER

// C通用布尔类型
#ifndef BOOL
typedef int BOOL;
#ifndef TRUE
static const int TRUE = 1;
static const int FALSE = 0;
#endif
#endif

// Debug断言宏的定义, 同AAFragment的同名函数
#ifndef AAAssert
#if defined DEBUG
#include <assert.h>
#define AAAssert(x,y) assert(x)
#else
#define AAAssert(x,y) if(!(x))return y;
#endif // ifdef DEBUG
#endif // ifndef AAAssert

#ifndef _cplusplus

// C99的布尔转C++布尔
typedef _Bool bool;
static const int true = TRUE;
static const int false = FALSE;

// C语言自然对数底
static const double s_aa_natrualBase = 2.7182818284590452353;

// C语言定义的宏, 同AAFragment的同名函数
#ifndef min
#define min(x,y) ((x>y)?y:x)
#endif
#ifndef max
#define max(x,y) ((x<y)?y:x)
#endif

// 安全释放内存(置空指针)宏定义, 同AAFragment
#define AA_SAFE_DEL(x) if(x != NULL){free(x); x = NULL;}else;
#define AA_SAFE_FREE AA_SAFE_DEL
#define AA_SAFE_DELALL AA_SAFE_DEL

#endif

#define ARMYANT_CLIB_API ARMYANTLIB_API

#endif // C_A_A_DEFINE_H_2015_11_11