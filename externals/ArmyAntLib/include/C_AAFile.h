#ifndef C_BS_FILE_H_2015_11_6
#define C_BS_FILE_H_2015_11_6

#include "C_AADefine.h"
#include "AA_start.h"

#ifdef _cplusplus
extern "C" {
#endif

	typedef enum AA_StreamType
	{
		None,
		File,
		Memory,
		NamePipe,
		ComData,
		Network
	} AA_StreamType;

	DWORD AA_FILE_MAX_LENGTH = 0xffffffff;
	typedef DWORD AA_CFileStream;

	ARMYANT_CLIB_API AA_CFileStream AA_Stream_Create();
	ARMYANT_CLIB_API void AA_Stream_Release(AA_CFileStream stream);
	ARMYANT_CLIB_API bool AA_Stream_SetMode(AA_CFileStream stream, bool nocreate, bool noexist);

	ARMYANT_CLIB_API bool AA_Stream_OpenFile(AA_CFileStream stream, const char* filename);
	ARMYANT_CLIB_API bool AA_Stream_OpenMemoryByAddr(AA_CFileStream stream, DWORD memaddr, WORD len);
	ARMYANT_CLIB_API bool AA_Stream_OpenMemoryByPtr(AA_CFileStream stream, void* memaddr, WORD len);
	ARMYANT_CLIB_API bool AA_Stream_OpenNamePipe(AA_CFileStream stream, const char* pipename, const char*pipePath, const char*pipeServer);
	ARMYANT_CLIB_API bool AA_Stream_OpenCom(AA_CFileStream stream, BYTE comNum);
	ARMYANT_CLIB_API bool AA_Stream_OpenNetWithName(AA_CFileStream stream, const char* netAddr, BYTE protocol);
	ARMYANT_CLIB_API bool AA_Stream_OpenNetWithAddr(AA_CFileStream stream, DWORD netIp, WORD port, BYTE protocol);
	ARMYANT_CLIB_API bool AA_Stream_Close(AA_CFileStream stream);

	ARMYANT_CLIB_API bool AA_Stream_IsOpened(AA_CFileStream stream);
	ARMYANT_CLIB_API AA_StreamType AA_Stream_NowType(AA_CFileStream stream);
	ARMYANT_CLIB_API DWORD AA_Stream_GetLength(AA_CFileStream stream);
	ARMYANT_CLIB_API DWORD AA_Stream_GetPos(AA_CFileStream stream);
	ARMYANT_CLIB_API bool AA_stream_IsEndPos(AA_CFileStream stream);
	ARMYANT_CLIB_API bool AA_Stream_MovePos(AA_CFileStream stream, DWORD pos);
	ARMYANT_CLIB_API const char* AA_Stream_GetStreamName(AA_CFileStream stream);

	ARMYANT_CLIB_API DWORD AA_Stream_ReadSome(AA_CFileStream stream, void*buffer, DWORD len, DWORD pos);
	ARMYANT_CLIB_API DWORD AA_Stream_ReadTo(AA_CFileStream stream, void*buffer, BYTE endtag, DWORD maxlen);
	ARMYANT_CLIB_API DWORD AA_Stream_Write(AA_CFileStream stream, void*buffer, DWORD len);

	ARMYANT_CLIB_API bool AA_Stream_CopyFile(const char*srcPath, const char*dstPath);
	ARMYANT_CLIB_API bool AA_Stream_MoveOrRenameFile(const char*srcPath, const char*dstPath);
	ARMYANT_CLIB_API bool AA_Stream_DeleteFile(const char*path);
	ARMYANT_CLIB_API bool AA_Stream_IsFileExist(const char*path);

#ifdef _cplusplus
}
#endif

#endif
