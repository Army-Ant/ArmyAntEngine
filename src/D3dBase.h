#ifndef D3D_BASE_H_2015_11_13
#define D3D_BASE_H_2015_11_13

#ifdef OS_WINDOWS
#include <windows.h>

#else
#error "The DirectX 11 cannot run in Unix, Linux, OSX, iOS, Android, or any other system which didn't use the Windows core"
#endif

#include "D3d11Use_start.h"
#include "Structure2D.h"
#include "../externals/ArmyAntLib/include/AADefine.h"
#include <xmemory>

struct D3D11_INPUT_ELEMENT_DESC;

namespace AA_Engine {

namespace AA_D3dRef {


class D3dBuffer;


enum class BufferType : BYTE
{
	Vertex,
	Picture,
	Alpha
};


class D3D11USE_API D3dBase
{
public:
	D3dBase(HWND window, DWORD bufferCount = 1, DWORD SampleDescCount = 1, DWORD SampleDescQuality = 0, DWORD fps = 60, DWORD width = 0, DWORD height = 0);
	virtual ~D3dBase();

public:
	DWORD CreateViewport();
	DWORD CreateViewport(float x, float y, float w, float h, float minDepth, float maxDepth);
	bool ResetViewport(AA_Engine::Algorithm::Color32 color32 = 0xffffffff);

	D3dBuffer* MakeBuffer(BufferType type, DWORD datalen, void*datas);
	D3dBuffer* ReleaseBuffer(D3dBuffer*buffer);

public:
	static const DWORD GetScreenWidth();
	static const DWORD GetScreenHeight();

public:
	const unsigned int handle;

protected:
	bool CreateDevice(HWND window, DWORD bufferCount, DWORD SampleDescCount, DWORD SampleDescQuality, DWORD fps, DWORD width, DWORD height);
	bool CreateBackBuffer();
	bool ReleaseBackBuffer();

	AA_FORBID_ASSGN_OPR(D3dBase);
	AA_FORBID_COPY_CTOR(D3dBase);
};


class D3D11USE_API D3dBuffer
{
public:
	D3dBuffer(const D3dBase&parent);
	D3dBuffer(const D3dBuffer&value);
	~D3dBuffer();
	D3dBuffer&operator =(const D3dBuffer&value);

public:
	bool SetType(BufferType type);
	bool SetDatas(DWORD datalen, void*datas);
	bool CreateBuffer();
	bool CreateShader(const char*shaderCodeFile, bool isVertexShader = true, const char* EntryPoint = nullptr);
	bool ReleaseShader(bool isVertexShader);
	bool CreateInputLayout(DWORD pointNums, AA_Engine::Algorithm::Color32 innerColor);
	bool Render();

public:
	const unsigned int handle;
};


class D3D11USE_API Texture
{
public:
	Texture();
	Texture(const Texture&value);
	Texture&operator=(const Texture&value);
	~Texture();

public:


public:
	const unsigned int handle;
};




} // namespace AA_D3dRef;

} // namespace AA_Engine

#endif