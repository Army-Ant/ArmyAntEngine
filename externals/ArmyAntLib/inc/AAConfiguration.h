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
#ifndef AA_CONFIGURATION_2016_3_28
#define AA_CONFIGURATION_2016_3_28

#include "AA_start.h"
#include "AATree.hpp"

namespace ArmyAnt {

class ARMYANTLIB_API Configuration
{
public:
	Configuration();
	Configuration(const Configuration&value);
	Configuration&operator=(const Configuration&value);
	virtual ~Configuration();

public:
	bool LoadFromIni(const char*iniStr);
	bool LoadFromXml(const char*xmlStr);
	bool LoadFromJson(const char*jsonStr);
	bool LoadFromAABinary(const void*bins, uint64 len);
	bool SaveAsIni(char*iniStr)const;
	bool SaveAsXml(char*xmlStr)const;
	bool SaveAsJson(char*jsonStr)const;
	uint64 SaveAsAABinary(void*bins)const;

public:
	const char* GetRootName()const;
	const char* GetRootValue()const;
	const char* GetParentName()const;
	Configuration* GetParent();
	const Configuration* GetParent()const;
	Configuration* GetChild(const char*childName);
	const Configuration* GetChild(const char*childName)const;
	uint32 GetMaxChildNameLength()const;
	uint32 GetChildren(char*const*childName)const;

	bool SetRootName(const char*name);
	bool SetRootValue(const char*value);
	bool AddChild(const char*name, const char*value);
	bool AddChild(const Configuration&child);
	bool RemoveChild(const char*name);
	bool RemoveChild(const Configuration&child);
	bool AddToParent(const Configuration&child);
	bool RemoveFromParent();
	// ArmyAnt::Tree<std::string, std::string>*
	void* GetTree();

public:
	const uint32 handle;
};

} // namespace ArmyAnt

#endif // AA_CONFIGURATION_2016_3_28