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
#include "include/AADefine.h"
// Template class of C++ class helper
#include "include/AAClassPrivateHandle.hpp"
// Template class for a map with two values
#include "include/AATripleMap.hpp"
// File stream class, to work in disk file, memory, name pipe, com, network
#include "include/AAFile.h"
// AES encoding and decoding class
#include "include/AAAes.h"
// Binary data packer and unpacker class, used to make file or socket datas for program
#include "include/AABinary.h"
// State Machine class, the key is a string
#include "include/AAStateMachine.h"



#include "include/AA_end.h"

#endif // ARMY_ANT_H_2015_11_11