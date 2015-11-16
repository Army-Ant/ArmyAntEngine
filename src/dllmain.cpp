#include <cstdarg>
#include "AA_D3D11_3_used.h"

#include <d3d11_2.h>
#include <d3dx11.h> 

//你的unresolved symbol是因为没有载入这个winSDK的lib
#pragma comment(lib,"winmm.lib")

int WINAPI DllMain(_In_ HANDLE _HDllHandle, _In_ DWORD _Reason, _In_opt_ LPVOID _Reserved)
{
	switch(_Reason)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}
	return 0;
}

static IDXGISwapChain* swapChain_;
static ID3D11Device* d3dDevice_;
static ID3D11DeviceContext* d3dContext_;
static unsigned int width;
static unsigned int height;
static D3D11_VIEWPORT viewport;
static ID3D11RenderTargetView* backBufferTarget_;

HRESULT CreateDevice(HWND hwnd)
{
	RECT dimensions;
	GetClientRect(hwnd, &dimensions);
	width = dimensions.right - dimensions.left;
	height = dimensions.bottom - dimensions.top;
	D3D_DRIVER_TYPE driverTypes[] = {D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE};
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_10_0};
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;		//缓存页面总数
	swapChainDesc.BufferDesc.Width = width;		//缓存页宽度
	swapChainDesc.BufferDesc.Height = height;	//缓存页高度
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//缓存格式
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;		//每周期刷新次数
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;		//周期长度(秒)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//缓存用法
	swapChainDesc.OutputWindow = hwnd;		//窗口句柄
	swapChainDesc.Windowed = true;		//全屏模式中，是否继续使用原先的尺寸
	swapChainDesc.SampleDesc.Count = 1;		//取样数量
	swapChainDesc.SampleDesc.Quality = 0;	//取样质量

	unsigned int creationFlags = 0;
#ifdef _DEBUG   
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif  
	HRESULT result;
	unsigned int driver = 0;
	unsigned int feature = 0;
	for(driver = 0; driver < totalDriverTypes; ++driver)
	{
		for(auto featureLevel_ = featureLevels[feature]; feature < totalFeatureLevels; featureLevel_ = featureLevels[++feature])
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags, featureLevels, totalFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &swapChain_, &d3dDevice_, &featureLevel_, &d3dContext_);
			if(SUCCEEDED(result))
			{
				auto driverType_ = driverTypes[driver];
				return result;
			}
		}
	}
	if(FAILED(result)) 
	{
		//DXTRACE_MSG("Failed to create the Direct3D device!");   
	}
	return result;
}

HRESULT CreateBuffer()
{
	ID3D11Texture2D* backBufferTexture;
	auto result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to get the swap chain back buffer!");
		return result;
	}
	result = d3dDevice_->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget_);
	if(backBufferTexture)
		backBufferTexture->Release();
	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to create the render target view!");  
		return result;
	}
	d3dContext_->OMSetRenderTargets(1, &backBufferTarget_, 0);
	return result;
}

void CreateViewport()
{
	viewport.Width = static_cast<float>(width);   
	viewport.Height = static_cast<float>(height);   
	viewport.MinDepth = 0.0f;   
	viewport.MaxDepth = 1.0f;   
	viewport.TopLeftX = 0.0f;  
	viewport.TopLeftY = 0.0f;
	d3dContext_->RSSetViewports(1, &viewport);
}

HRESULT ResetTargetViewport()
{
	float clearColor[4] = {0.0f, 0.0f, 0.25f, 1.0f};
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
	return swapChain_->Present(0, 0);
}

void ReleaseD3D()
{
	if(backBufferTarget_)
		backBufferTarget_->Release();
	if(swapChain_)
		swapChain_->Release();
	if(d3dContext_)
		d3dContext_->Release();
	if(d3dDevice_)
		d3dDevice_->Release();
}