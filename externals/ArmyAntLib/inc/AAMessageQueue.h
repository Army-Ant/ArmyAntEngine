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

#ifndef AA_MESSAGE_QUEUE_H_2016_1_22
#define AA_MESSAGE_QUEUE_H_2016_1_22

/*	* @ author			: Jason
	* @ date			: 01/22/2016
	* @ nearly update	: 01/22/2015
	* @ small version	: 0.1
	* @ summary			: 消息队列
	* @ uncompleted		: all
	* @ untested		:
	* @ tested			:
	*/

#include <functional>
#include "AADefine.h"

namespace ArmyAnt {

typedef std::function<void(uint32 eventid, void*params)> EventCall;

class MessageQueue
{
public:
	MessageQueue(EventCall onevent);
	~MessageQueue();

public:
	bool RegisterEvent(uint32 eventid);
	bool UnRegisterEvent(uint32 eventid);
	bool IsEventRegistered(uint32 eventid);
	bool EnableEvent(uint32 eventid);
	bool DisableEvent(uint32 eventid);
	bool IsEventDisabled(uint32 eventid);
	bool DispatchEvent(uint32 eventid, void*params);

	AA_FORBID_ASSGN_OPR(MessageQueue);
	AA_FORBID_COPY_CTOR(MessageQueue);
};

} // namespace ArmyAnt

#endif // AA_MESSAGE_QUEUE_H_2016_1_22