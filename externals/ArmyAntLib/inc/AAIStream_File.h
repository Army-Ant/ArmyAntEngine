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

#ifndef AA_FILE_H_2015_11_11
#define AA_FILE_H_2015_11_11

/*	* @ author			: Jason
	* @ date			: 10/17/2015
	* @ nearly update	: 11/12/2015
	* @ small version	: 0.1
	* @ summary			: 流操作类的文件操作派生类，可以对文件进行打开、关闭、读写
	* @ uncompleted		: 							
	* @ untested		: 磁盘文件的静态操作函数（拷贝、移动、重命名、删除等功能）
	* @ tested			: 磁盘文件的基本操作，包括打开关闭，判断是否打开，读写指针操作，读取和写入
	* @ issue			: 打开一个不存在的磁盘文件（新建）后，写入时必然失败
	*/

#include <cstdio>
#include "AAIStream.h"

namespace ArmyAnt {


class ARMYANTLIB_API File : public StaticStream
{
public:
	File();
	~File();

public:
	/* 设定流的打开规则
	 * @ param = "nocreate" : 若此参数为true，则不允许创建新文件，文件不存在时会打开失败
	 * @ param = "noexist" : 若此参数为true，则不允许文件已存在，文件已存在时会打开失败
	 */
	bool SetStreamMode(bool nocreate = true, bool noexist = false);	

	/* 打开磁盘文件
	 * @ param = "src" : 要打开的文件路径
	 */
	virtual bool Open(const char* src) override;

	/* 关闭流，任何类型的流都要用此方式关闭
	 */
	virtual bool Close() override;

	/* 检验流是否打开中
	 * @ param = "dynamicCheck" : 是否进行实时检查，当该参数为默认或者true时，在非联网状态下，将尝试读取流，联网状态下，将尝试ping目标地址
	 */
	virtual bool IsOpened(bool dynamicCheck = true) override;

	/* 检验当前流的类型
	 * @ return : 返回流的类型
	 */
	virtual StreamType GetType() const final { return StreamType::File; };

	/* 检查流的长度
	 */
	virtual uint64 GetLength() const override;

	/* 检查当前读写指针的位置
	 */
	virtual uint64 GetPos() const override;

	/* 读写指针是否已到流末尾
	 */
	virtual bool IsEndPos() const override;

	/* 将读写指针移动到指定位置
	 * @ param = "pos" : 从流开头算起，要移动到的位置。此参数默认值为移动到流尾部
	 */
	virtual bool MovePos(uint64 pos = AA_UINT64_MAX) override;

	/* 获取流源的文件名，对于文件指其路径名称，对于管道则是管道全名，对于串口则是串口名，对于内存和共享内存，则是内存地址号，对于网络，则是其域名或者ip地址和端口
	 */
	virtual const char* GetSourceName() const override;

	/* 读取流中的数据
	 * @ param = "buffer" : 要将数据保存到的位置
	 * @ param = "len" : 要读取的最大长度
	 * @ param = "pos" : 要读取的开始位置，不传此参数则从当前位置就地读取
	 * @ return : 读取到的实际长度，如果为0，可能发生了错误
	 */
	virtual uint64 Read(void*buffer, uint32 len = AA_UINT32_MAX, uint64 pos = AA_UINT64_MAX)override;

	/* 读取流中的数据
	 * @ param = "buffer" : 要将数据保存到的位置
	 * @ param = "endtag" : 读取到此值的字节数据时，停止
	 * @ param = "maxlen" : 要读取的最大长度
	 * @ return : 读取到的实际长度，如果为0，可能发生了错误
	 */
	virtual uint64 Read(void*buffer, uint8 endtag, uint64 maxlen = AA_UINT64_MAX)override;

	/* 将数据写入流
	 * @ param = "buffer" : 要写入的数据所在的位置
	 * @ param = "len" : 要写入的长度，不传此参数，则当遇到数据中的0值（字符串结尾）时停止写入
	 * @ return : 写入的实际长度，如果为0，可能发生了错误
	 */
	virtual uint64 Write(void*buffer, uint64 len = 0)override;

	virtual bool IsEmpty() const override;

public:
	/* 拷贝文件，要求目标文件不存在，否则返回false
	 * @ param = "srcPath" : 源文件路径
	 * @ param = "dstPath" : 目标文件路径
	 */
	static bool CopyFile(const char*srcPath, const char*dstPath);

	/* 移动文件或重命名文件，要求目标文件或文件名不存在，否则返回false
	 * @ param = "srcPath" : 源文件路径名称
	 * @ param = "dstPath" : 目标文件路径名称
	 */
	static bool MoveOrRenameFile(const char*srcPath, const char*dstPath);

	/* 删除文件
	 * @ param = "path" : 要删除的文件路径
	 */
	static bool DeleteFile(const char*path);

	/* 检查文件是否已存在
	 * @ param = "path" : 要检查的文件路径
	 */
	static bool IsFileExist(const char*path);

public:
	/* 将流内容从当前指针处之后的所有内容，拷贝到另一文件
	 * @ param = "filename" : 要写入的文件
	 */
	bool operator^=(const char* filename);

public:
	static File*GetStream(uint32 handle);

	AA_FORBID_ASSGN_OPR(File);
	AA_FORBID_COPY_CTOR(File);
};

} // namespace ArmyAnt

#endif // AA_FILE_H_2015_11_11
