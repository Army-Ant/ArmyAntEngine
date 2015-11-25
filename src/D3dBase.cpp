#include "base.hpp"
#include "D3dBase.h"
// #include "../externals/ArmyAntLib/ArmyAnt.h"
#include <d3dcompiler.h>

namespace AA_Engine {

namespace AA_D3dRef {


class D3dBase_private
{
public:
	D3dBase_private() {}
	~D3dBase_private() {}

	IDXGISwapChain* swapChain_ = nullptr;
	ID3D11Device* d3dDevice_ = nullptr;
	ID3D11DeviceContext* d3dContext_ = nullptr;
	unsigned int width = 0;
	unsigned int height = 0;
	
	ID3D11RenderTargetView* backBuffer;

	AA_FORBID_COPY_CTOR(D3dBase_private);
	AA_FORBID_ASSGN_OPR(D3dBase_private);
};

static ArmyAnt::ClassPrivateHandleManager<D3dBase, D3dBase_private, unsigned int> handleManager;

D3dBase::D3dBase(HWND window, DWORD bufferCount /* = 1 */, DWORD SampleDescCount /* = 1 */, DWORD SampleDescQuality /* = 0 */, DWORD fps /* = 60 */, DWORD width /* = 0 */, DWORD height /* = 0 */)
	:handle(handleManager.GetHandle(this))
{
	AAAssert(CreateDevice(window, bufferCount, SampleDescCount, SampleDescQuality, fps, width, height));
	AAAssert(CreateBackBuffer());
}

D3dBase::~D3dBase()
{
	auto hd = handleManager.GetDataByHandle(handle);
	AAAssert(ReleaseBackBuffer());
	if(hd->swapChain_)
		hd->swapChain_->Release();
	if(hd->d3dContext_)
		hd->d3dContext_->Release();
	if(hd->d3dDevice_)
		hd->d3dDevice_->Release();
	handleManager.ReleaseHandle(handle);
}

DWORD D3dBase::CreateViewport()
{
	auto hd = handleManager.GetDataByHandle(handle);
	return CreateViewport(0.0f, 0.0f, static_cast<float>(hd->width), static_cast<float>(hd->height), 0.0f, 1.0f);
}


DWORD D3dBase::CreateViewport(float x, float y, float w, float h, float minDepth, float maxDepth)
{
	auto hd = handleManager.GetDataByHandle(handle);
	D3D11_VIEWPORT viewport;
	viewport.Width = w;
	viewport.Height = h;
	viewport.MinDepth = minDepth;
	viewport.MaxDepth = maxDepth;
	viewport.TopLeftX = x;
	viewport.TopLeftY = y;
	hd->d3dContext_->RSSetViewports(1, &viewport);
	return true;

}

bool D3dBase::ResetViewport()
{
	auto hd = handleManager.GetDataByHandle(handle);
	float clearColor[4] = {0.0f, 0.0f, 0.25f, 1.0f};
	hd->d3dContext_->ClearRenderTargetView(hd->backBuffer, clearColor);
	return 0 <= hd->swapChain_->Present(0, 0);
}


D3dBuffer* D3dBase::CreateBuffer(BufferType type, DWORD datalen, void*datas)
{
	auto ret = new D3dBuffer(*this);
	ret->SetType(type);
	ret->SetDatas(datalen, datas);
	if(!ret->CreateBuffer())
		AA_SAFE_DEL(ret);
	return ret;
}

D3dBuffer* D3dBase::ReleaseBuffer(D3dBuffer*buffer)
{
	AA_SAFE_DEL(buffer);
	return buffer;
}

const DWORD D3dBase::GetScreenWidth()
{
	return GetSystemMetrics(SM_CXSCREEN);
}


const DWORD D3dBase::GetScreenHeight()
{
	return GetSystemMetrics(SM_CYSCREEN);
}

bool D3dBase::CreateDevice(HWND window, DWORD bufferCount, DWORD SampleDescCount, DWORD SampleDescQuality, DWORD fps, DWORD width, DWORD height)
{
	auto hd = handleManager.GetDataByHandle(handle);

	hd->width = width;
	hd->height = height;
	D3D_DRIVER_TYPE driverTypes[] = {D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE};
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_10_0};
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = bufferCount;		//缓存页面总数
	swapChainDesc.BufferDesc.Width = hd->width;		//缓存页宽度
	swapChainDesc.BufferDesc.Height = hd->height;	//缓存页高度
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//缓存格式
	swapChainDesc.BufferDesc.RefreshRate.Numerator = fps;		//每周期刷新次数
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;		//周期长度(秒)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//缓存用法
	swapChainDesc.OutputWindow = window;		//窗口句柄
	swapChainDesc.Windowed = true;		//全屏模式中，是否继续使用原先的尺寸
	swapChainDesc.SampleDesc.Count = SampleDescCount;		//取样数量
	swapChainDesc.SampleDesc.Quality = SampleDescQuality;	//取样质量

	unsigned int creationFlags = 0;
#ifdef _DEBUG   
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif  
	HRESULT result = -1;
	for(unsigned int driver = 0; driver < totalDriverTypes; ++driver)
	{
		unsigned int feature = 0;
		for(auto featureLevel_ = featureLevels[feature]; feature < totalFeatureLevels; featureLevel_ = featureLevels[++feature])
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags, featureLevels, totalFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &hd->swapChain_, &hd->d3dDevice_, &featureLevel_, &hd->d3dContext_);
			if(SUCCEEDED(result))
			{
				//auto driverType_ = driverTypes[driver];
				return result >= 0;
			}
		}
	}
	return result >= 0;
}

bool D3dBase::CreateBackBuffer()
{
	auto hd = handleManager.GetDataByHandle(handle);
	ID3D11Texture2D* bufferTexture;
	auto result = hd->swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&bufferTexture);
	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to get the swap chain back buffer!");
		return false;
	}
	result = hd->d3dDevice_->CreateRenderTargetView(bufferTexture, 0, &hd->backBuffer);
	if(bufferTexture)
		bufferTexture->Release();
	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to create the render target view!");  
		return false;
	}
	hd->d3dContext_->OMSetRenderTargets(1, &hd->backBuffer, nullptr);
	return true;

}

bool D3dBase::ReleaseBackBuffer()
{
	return  handleManager.GetDataByHandle(handle)->backBuffer->Release() >= 0;
}


/************************** Source Code : D3dBuffer ***********************************************/


class D3dBuffer_Private
{
public:	
	D3dBuffer_Private() {};
	~D3dBuffer_Private() {};

public:
	const D3dBase*parent = nullptr;
	BufferType type = BufferType::Vertex;
	void*datas = nullptr;
	DWORD datalen = 0;
	D3D11_BUFFER_DESC bufferDesc; 
	ID3D11Buffer* vertexBuffer = nullptr;
	bool isBufferCreated = false;
	bool isShaderCreated = false;

	AA_FORBID_COPY_CTOR(D3dBuffer_Private);
	AA_FORBID_ASSGN_OPR(D3dBuffer_Private);
};

static ArmyAnt::ClassPrivateHandleManager<D3dBuffer, D3dBuffer_Private, unsigned int> bufferHandleManager;

D3dBuffer::D3dBuffer(const D3dBase&parent) 
	:handle(bufferHandleManager.GetHandle(this))
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	hd->parent = &parent;
}

D3dBuffer::D3dBuffer(const D3dBuffer&value)
	:handle(bufferHandleManager.GetHandle(this))
{
	*this = value;
}

D3dBuffer::~D3dBuffer()
{
	bufferHandleManager.ReleaseHandle(handle);
}

D3dBuffer& D3dBuffer::operator=(const D3dBuffer&value)
{
	// TODO COPY
	return *this;
}

bool D3dBuffer::SetType(BufferType type)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	hd->type = type;
	return true;
}


bool D3dBuffer::SetDatas(DWORD datalen, void*datas)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	hd->datas = datas;
	hd->datalen = datalen;
	return datas != nullptr && datalen > 0;
}

bool D3dBuffer::CreateBuffer()
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	hd->bufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	hd->bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	hd->bufferDesc.ByteWidth = hd->datalen;
	D3D11_SUBRESOURCE_DATA data; 
	memset(hd->datas, 0, hd->datalen);
	data.pSysMem = hd->datas;
	return hd->isBufferCreated = (0 <= handleManager.GetDataByHandle(hd->parent->handle)->d3dDevice_->CreateBuffer(&hd->bufferDesc, &data, &hd->vertexBuffer));
}

bool D3dBuffer::CreateShader()
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	auto phd = handleManager.GetDataByHandle(hd->parent->handle);

	ID3D11VertexShader* solidColorVS; 
	ID3D11PixelShader* solidColorPS; 
	ID3D11InputLayout* inputLayout;
	ID3DBlob* vsBuffer = 0;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG; 
#endif  
	ID3DBlob* errorBuffer = 0; 
	HRESULT result;
	result = D3DX11CompileFromFile("D3D11Shader.hlsl", 0, 0, "VertexShaderMain", "vs_5_0", shaderFlags, 0, 0, &vsBuffer, &errorBuffer, 0);
	if(FAILED(result)) 
	{ 
		if(errorBuffer != 0) 
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}
		return false;
	}
	if(errorBuffer != 0) 
		errorBuffer->Release();
	result = phd->d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &solidColorVS);
	if(FAILED(result)) 
	{
		if(vsBuffer) vsBuffer->Release();
		return false; 
	}
	return true;
}

}

}