#ifndef D3D_BASE_H_2015_11_13
#define D3D_BASE_H_2015_11_13

#ifdef OS_WINDOWS
#include <windows.h>

#else
#error "The DirectX 11 cannot run in Unix, Linux, OSX, iOS, Android, or any other system which didn't use the Windows core"
#endif

#include <xmemory>
#include <AADefine.h>
#include "D3d11Use_start.h"
#include "Structure2D.h"

struct D3D11_INPUT_ELEMENT_DESC;

namespace AA_Engine {

namespace AA_D3dRef {


class D3dBuffer;
class Texture2D;
enum class TextureFileType : uint8
{
	Null,
	BMP,
	TIFF,
	JPEG,
	GIF,
	DDS,
	WMP,
	PNG
};

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
	bool CreateViewport();
	bool CreateViewport(float x, float y, float w, float h, float minDepth, float maxDepth);
	bool ResetViewport(AA_Engine::Algorithm::Color32 color32 = 0xffffffff);

	D3dBuffer* MakeBuffer(BufferType type, DWORD datalen, void*datas);
	bool ReleaseBuffer(D3dBuffer*buffer);
	Texture2D* CreateTexture();
	Texture2D* CreateTexture(const char*filename, TextureFileType type);
	bool RemoveTexture(Texture2D*texture);

public:
	static const DWORD GetScreenWidth();
	static const DWORD GetScreenHeight();

public:
	const uint32 handle;

protected:
	bool CreateDevice(HWND window, DWORD bufferCount, DWORD SampleDescCount, DWORD SampleDescQuality, DWORD fps, DWORD width, DWORD height, const int* sparedSoftware = nullptr);
	bool CreateBackBuffer();
	bool ReleaseBackBuffer();

	AA_FORBID_ASSGN_OPR(D3dBase);
	AA_FORBID_COPY_CTOR(D3dBase);
};


class D3D11USE_API D3dBuffer
{
public:
	D3dBuffer(const D3dBase&parent);
	~D3dBuffer();

public:
	bool SetType(BufferType type);
	bool SetDatas(DWORD datalen, void*datas);
	bool CreateBuffer();
	bool CreateShader(const char*shaderCodeFile, bool isVertexShader = true, const char* EntryPoint = nullptr);
	bool ReleaseShader(bool isVertexShader);
	bool CreateInputLayout(DWORD pointNums, AA_Engine::Algorithm::Color32 innerColor);
	bool Render();
	D3dBase& GetDevice();
	const D3dBase& GetDevice()const;

public:
	const uint32 handle;
};


class D3D11USE_API Texture2D
{
public:
	Texture2D(const D3dBase&parent);
	Texture2D(const D3dBase&parent, const char*filename, TextureFileType type);
	Texture2D(const Texture2D&value);
	Texture2D&operator=(const Texture2D&value);
	~Texture2D();

public:
	bool LoadFromFile(const char*filename, TextureFileType type);

public:
	const uint32 handle;
};




} // namespace AA_D3dRef;

} // namespace AA_Engine

#endif