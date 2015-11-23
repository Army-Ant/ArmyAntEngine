#ifndef D3D_BASE_H_2015_11_13
#define D3D_BASE_H_2015_11_13

#ifdef OS_WINDOWS
#include <windows.h>

#elif defined OS_UNIX
#error "The DirectX 11 cannot run in Unix, Linux, OSX, iOS, Android, or any other system which used the Unix core"

#endif

#include "D3d11Use_start.h"
#include "../externals/ArmyAntLib/include/AADefine.h"

namespace AA_Engine {

namespace AA_D3dRef {

class D3D11USE_API D3dBase
{
public:
	D3dBase(HWND window, DWORD bufferCount = 1, DWORD SampleDescCount = 1, DWORD SampleDescQuality = 0, DWORD fps = 60, DWORD width = 0, DWORD height = 0);
	virtual ~D3dBase();

public:
	DWORD CreateBuffer();
	bool ReleaseBuffer(DWORD bufferHandle);
	bool ReleaseAllBuffers();
	DWORD CreateViewport(DWORD buffer);
	DWORD CreateViewport(DWORD buffer, float x, float y, float w, float h, float minDepth, float maxDepth);
	bool ResetViewport(DWORD viewPortHandle);

public:
	static DWORD GetScreenWidth();
	static DWORD GetScreenHeight();

public:
	const unsigned int handle;

protected:
	bool CreateDevice(HWND window, DWORD bufferCount, DWORD SampleDescCount, DWORD SampleDescQuality, DWORD fps, DWORD width, DWORD height);

	AA_FORBID_EQUAL_OPR(D3dBase);
	AA_FORBID_COPY_CTOR(D3dBase);
};

}

}

#endif