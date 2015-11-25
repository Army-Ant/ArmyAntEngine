#include "base.hpp"
#include "D3dBase.h"
#include "../externals/ArmyAntLib/ArmyAnt.h"

namespace AA_Engine {

namespace AA_D3dRef {

typedef std::pair<ID3D11RenderTargetView*, BYTE> BufferViewportValue;
typedef ArmyAnt::TripleMap<DWORD, ID3D11RenderTargetView*, BYTE> BufferViewportMap;

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
	AA_FORBID_EQUAL_OPR(D3dBase_private);
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
	ID3D11RenderTargetView* backBufferTarget_ = nullptr;
	result = hd->d3dDevice_->CreateRenderTargetView(bufferTexture, 0, &backBufferTarget_);
	if(bufferTexture)
		bufferTexture->Release();
	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to create the render target view!");  
		return false;
	}
	hd->d3dContext_->OMSetRenderTargets(1, &hd->backBuffer, 0);
	return true;

}

bool D3dBase::ReleaseBackBuffer()
{
	return  handleManager.GetDataByHandle(handle)->backBuffer->Release() >= 0;
}


}

}