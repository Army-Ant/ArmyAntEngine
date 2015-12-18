#ifndef AA_FILE_H_2015_11_11
#define AA_FILE_H_2015_11_11

/*	* @ author			: Jason
	* @ date			: 10/17/2015
	* @ nearly update	: 11/12/2015
	* @ small version	: 0.1
	* @ summary			: 流操作类，可以对流进行打开、关闭、读写，可用流包含磁盘文件、命名管道、串口、网络、内存或共享内存
	* @ uncompleted		: 网络通信的任何功能均未完成
							命名管道的方案没有完成
	* @ untested		: 磁盘文件的静态操作函数（拷贝、移动、重命名、删除等功能）
							命名管道的所有功能
	* @ tested			: 磁盘文件的基本操作，包括打开关闭，判断是否打开，读写指针操作，读取和写入
							内存或共享内存的所有功能
							串口的所有功能
	* @ issue			: 打开一个不存在的磁盘文件（新建）后，写入时必然失败
	*/

#include <cstdio>
#include "AADefine.h"
#include "AA_start.h"

namespace ArmyAnt {

enum class StreamType : BYTE
{
	None,
	File,
	Memory,
	NamePipe,
	ComData,
	Network		//网络通信功能尚未开发
};

class ARMYANTLIB_API FileStream
{
public:
	FileStream();
	~FileStream();

public:
	/*	* @ summary : 设定流的打开规则
		* @ param = "nocreate" : 若此参数为true，则不允许创建新文件，文件不存在时会打开失败
		* @ param = "noexist" : 若此参数为true，则不允许文件已存在，文件已存在时会打开失败
		*/
	bool SetStreamMode(bool nocreate = true, bool noexist = false);	

	/*	* @ summary : 打开磁盘文件
		* @ param = "filepath" : 要打开的文件路径
		*/
	bool Open(const char* filepath);

	/*	* @ summary : 按内存地址数据打开内存或共享内存
		* @ param = "memaddr" : 要内存地址编号
		* @ param = "len" : 可读写的内存块总大小
		*/
	bool Open(DWORD memaddr, fpos_t len);

	/*	* @ summary : 按内存指针打开内存或共享内存
		* @ param = "memaddr" : 要内存起始指针
		* @ param = "len" : 可读写的内存块总大小
		*/
	bool Open(BYTE* memaddr, fpos_t len);

	/*	* @ summary : 打开命名管道
		* @ param = "pipename" : 管道名，自定义
		* @ param = "pipePath" : 管道路径，可为空
		* @ param = "pipeServer" : 管道所在机器，通常为本机（默认）
	*/
	bool Open(const char* pipename, const char*pipePath, const char*pipeServer = ".");

	/*	* @ summary : 打开串口
		* @ param = "comNum" : 串口序号
	*/
	bool Open(BYTE comNum);

	//打开指定域名的网络通信（暂未实现）
	bool Open(const char* netAddr, BYTE protocol);
	//打开指定IP和端口的网络通信（暂未实现）
	bool Open(DWORD netIp, WORD port, BYTE protocol);

	/*	* @ summary : 关闭流，任何类型的流都要用此方式关闭
		*/
	bool Close();

	/*	* @ summary : 检验流是否打开中
		* @ param = "dynamicCheck" : 是否进行实时检查，当该参数为默认或者true时，在非联网状态下，将尝试读取流，联网状态下，将尝试ping目标地址
		*/
	bool IsOpened(bool dynamicCheck = true);

	/*	* @ summary : 检验当前流的类型
		* @ return : 返回流的类型
		*/
	StreamType NowType() const;

	/*	* @ summary : 检查流的长度
		*/
	fpos_t GetLength() const;

	/*	* @ summary : 检查当前读写指针的位置
		*/
	fpos_t GetPos() const;

	/*	* @ summary : 读写指针是否已到流末尾
		*/
	bool IsEndPos() const;

	/*	* @ summary : 将读写指针移动到指定位置
		* @ param = "pos" : 从流开头算起，要移动到的位置。此参数默认值为移动到流尾部
		*/
	bool MovePos(fpos_t pos = FILE_LONG_POS_END) const;

	/*	* @ summary : 获取流源的文件名，对于文件指其路径名称，对于管道则是管道全名，对于串口则是串口名，对于内存和共享内存，则是内存地址号，对于网络，则是其域名或者ip地址和端口
		*/
	const char* GetSourceName() const;

	/*	* @ summary : 读取流中的数据
		* @ param = "buffer" : 要将数据保存到的位置
		* @ param = "len" : 要读取的最大长度
		* @ param = "pos" : 要读取的开始位置，不传此参数则从当前位置就地读取
		* @ return : 读取到的实际长度，如果为0，可能发生了错误
		*/
	DWORD Read(void*buffer, DWORD len = FILE_SHORT_POS_END, fpos_t pos = FILE_LONG_POS_END);

	/*	* @ summary : 读取流中的数据
		* @ param = "buffer" : 要将数据保存到的位置
		* @ param = "endtag" : 读取到此值的字节数据时，停止
		* @ param = "maxlen" : 要读取的最大长度
		* @ return : 读取到的实际长度，如果为0，可能发生了错误
		*/
	DWORD Read(void*buffer, BYTE endtag, DWORD maxlen = FILE_SHORT_POS_END);

	/*	* @ summary : 将数据写入流
		* @ param = "buffer" : 要写入的数据所在的位置
		* @ param = "len" : 要写入的长度，不传此参数，则当遇到数据中的0值（字符串结尾）时停止写入
		* @ return : 写入的实际长度，如果为0，可能发生了错误
		*/
	DWORD Write(void*buffer, DWORD len = 0);

public:
	//以下仅限文件操作

	/*	* @ summary : 拷贝文件，要求目标文件不存在，否则返回false
		* @ param = "srcPath" : 源文件路径
		* @ param = "dstPath" : 目标文件路径
		*/
	static bool CopyFile(const char*srcPath, const char*dstPath);

	/*	* @ summary : 移动文件或重命名文件，要求目标文件或文件名不存在，否则返回false
		* @ param = "srcPath" : 源文件路径名称
		* @ param = "dstPath" : 目标文件路径名称
		*/
	static bool MoveOrRenameFile(const char*srcPath, const char*dstPath);

	/*	* @ summary : 删除文件
		* @ param = "path" : 要删除的文件路径
		*/
	static bool DeleteFile(const char*path);

	/*	* @ summary : 检查文件是否已存在
		* @ param = "path" : 要检查的文件路径
		*/
	static bool IsFileExist(const char*path);

public:

	/*	* @ summary : 将流内容写入指定位置，写入部分为从当前指针处到文件结尾，如果在到达结尾前已经写入的长度达到FILE_SHORT_POS_END，则停止
		* @ param = "buffer" : 要写入的区域
		*/
	FileStream&operator>>(void*buffer);

	/*	* @ summary : 将目标字符串内容写入流，该内容遇到0数据（字符串结尾）时停止写入
		* @ param = "buffer" : 要写入的区域
		*/
	FileStream&operator<<(void*buffer);

	/*	* @ summary : 将流内容从当前指针处之后的所有内容，拷贝到另一文件
		* @ param = "filename" : 要写入的文件
		*/
	bool operator^=(const char* filename);

	/*	* @ summary : 判断流是否为空
	*/
	bool operator==(std::nullptr_t);

	/*	* @ summary : 判断流是否不为空
	*/
	bool operator!=(std::nullptr_t);

public:
	// 最大文件长度
	static const fpos_t FILE_LONG_POS_END = 0x7fffffffffffffff;
	static const DWORD FILE_SHORT_POS_END = 0xffffffff;

public:
	// 内部数据句柄，对外无用
	const DWORD handle;
	static FileStream*GetStream(DWORD handle);

	AA_FORBID_ASSGN_OPR(FileStream);
	AA_FORBID_COPY_CTOR(FileStream);
};

} // namespace ArmyAnt

#endif // AA_FILE_H_2015_11_11
