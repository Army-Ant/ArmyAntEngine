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

/*	*
	* Created by Jason ZJ 10/17/2015

	* You need only include this header file when you use this library
	* 要使用本库，你仅需要包含此头文件
	* You can also comment those header files you do not want to use which is referenced in this file
	* 本头文件包含了此库的成员模块，如果其中有你不想使用的部分，你也可以注释掉
	* For more information, please read those header files
	* 想要获取更多信息，请参考这些模块的头文件
 *	*/

#ifndef ARMY_ANT_H_2015_11_11
#define ARMY_ANT_H_2015_11_11

// Base symbol defination
#include "AADefine.h"
// Base information interface
#include "ArmyAntLib.h"

// Template class of C++ class helper
#include "AAClassPrivateHandle.hpp"
// Template class for a map with two values
#include "AATripleMap.hpp"
// Template class for tree
#include "AATree.hpp"
// Template class for digraph
#include "AADigraph.hpp"
// Template class for state machine
#include "AAStateMachine.hpp"
// Template class for neuron struct
#include "AANeuron.hpp"

// File stream class, to work in disk file, memory, name pipe, com, network
#include "AAIStream.h"
#include "AAIStream_File.h"
#include "AAIStream_Pipe.h"
#include "AAIStream_Memory.h"
#include "AAIStream_Com.h"
// Java native interface using tools
#include "AAJNITools.h"
// AES encoding and decoding class
#include "AAAes.h"
// Binary data packer and unpacker class, used to make file or socket datas for program
#include "AABinary.h"
// Message queue
#include "AAMessageQueue.h"
// Socket
#include "AASocket.h"
// Configuration text (xml, ini, json)
#include "AAConfiguration.h"



#include "AA_end.h"

#endif // ARMY_ANT_H_2015_11_11