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

#ifndef AA_BINARY_H_2015_12_8
#define AA_BINARY_H_2015_12_8

/*	* @ author			: Jason
	* @ date			: 12/08/2015
	* @ nearly update	: 12/08/2015
	* @ small version	: 0.1
	* @ summary			: 二进制数据转换器，转换的数据用于文件或网络通信
	* @ uncompleted		: operator overload
	* @ untested		:
	* @ tested			:
	*/

#include "AADefine.h"
#include "AA_start.h"

namespace ArmyAnt {

class ARMYANTLIB_API BinaryParser
{
public:
	BinaryParser();
	~BinaryParser();

public:
	uint64 GetBytes(void* buffer)const;
	bool SetBytes(const void* buffer, uint32 len);

	bool SetInfo(const char*tag, bool value);
	bool SetInfo(const char*tag, uint64 value, uint8 bits = 32);
	bool SetInfo(const char*tag, char value);
	bool SetInfo(const char*tag, double value);
	bool SetInfo(const char*tag, const char*value);
	bool SetInfo(const char*tag, void*values, uint32 len);
	template <class T>
	inline bool SetInfo(const char*tag, const T& values) { return SetInfo(tag, &values, sizeof(T)); }

	bool GetBoolInfo(const char*tag)const;
	char GetInt8Info(const char*tag)const;
	uint8 GetUInt8Info(const char*tag)const;
	int16 GetInt16Info(const char*tag)const;
	uint16 GetUInt16Info(const char*tag)const;
	int32 GetInt32Info(const char*tag)const;
	uint32 GetUInt32Info(const char*tag)const;
	int64 GetInt64Info(const char*tag)const;
	uint64 GetUInt64Info(const char*tag)const;
	double GetFloatInfo(const char*tag)const;
	const char* GetStrInfo(const char*tag, char*buffer)const;
	const void* GetBinaryInfo(const char*tag, void*buffer, uint32&len)const;
	template <class T>
	inline T GetStructInfo(const char*tag)const { T ret; return GetBinaryInfo(tag, &ret, sizeof(T)); }

	const char* GetDataType(const char*tag)const;
	bool RemoveInfo(const char*tag);
	bool ClearInfo();
	bool IsEmpty()const;

public:
	const uint64 handle;

	AA_FORBID_COPY_CTOR(BinaryParser);
	AA_FORBID_ASSGN_OPR(BinaryParser);
};

}

#endif // AA_BINARY_H_2015_12_8