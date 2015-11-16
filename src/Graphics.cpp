#include "Graphics.h"

namespace AA_Engine {

namespace AA_Graphic {

Graphics::Graphics()
{
	m_driverType = D3D_DRIVER_TYPE_NULL;
	m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	m_pD3dDevice = nullptr;
	m_pD3dContext = nullptr;
	m_pSwapChain = nullptr;
	m_pBackBufferTarget = nullptr;
}
Graphics::~Graphics()
{
	ReleaseAll();
}
bool Graphics::Initialize(HINSTANCE hinstance, HWND hw)
{
	m_hInstance = hinstance;
	m_hwnd = hw;
	//===========================================================
	// Specifying the driver type and feature levels.
	//===========================================================
	RECT dimensions;
	GetClientRect(m_hwnd, &dimensions);

	unsigned int width = dimensions.right - dimensions.left;
	unsigned int height = dimensions.bottom - dimensions.top;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
	};

	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);
	//===========================================================
	// The swap chain description
	//===========================================================
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	unsigned int creationFlags = 0;
	//===========================================================
	//Create the Direct3D device, context, and swap chain
	//===========================================================
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result;
	unsigned int driver = 0;

	for(driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags,
											   featureLevels, totalFeatureLevels,
											   D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain,
											   &m_pD3dDevice, &m_featureLevel, &m_pD3dContext);

		if(SUCCEEDED(result))
		{
			m_driverType = driverTypes[driver];
			break;
		}
	}

	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to create the Direct3D device!");
		return false;
	}
	//=================================================================
	//Render Target View Creation
	//==============================================================
	ID3D11Texture2D* backBufferTexture;

	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to get the swap chain back buffer!");
		return false;
	}

	result = m_pD3dDevice->CreateRenderTargetView(backBufferTexture, 0, &m_pBackBufferTarget);

	if(backBufferTexture)
		backBufferTexture->Release();

	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to create the render target view!");
		return false;
	}

	m_pD3dContext->OMSetRenderTargets(1, &m_pBackBufferTarget, 0);
	//===================================================================
	//The creation and setting of a full-screen viewport
	//===================================================================
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_pD3dContext->RSSetViewports(1, &viewport);
	//======================================================================
	return LoadContent();
}
void Graphics::ReleaseAll()
{
	UnloadContent();
	if(m_pBackBufferTarget)
		m_pBackBufferTarget->Release();
	if(m_pSwapChain)
		m_pSwapChain->Release();
	if(m_pD3dContext)
		m_pD3dContext->Release();

	m_pBackBufferTarget = nullptr;
	m_pSwapChain = nullptr;
	m_pD3dContext = nullptr;
}
void Graphics::UnloadContent()
{
	//.........
}
bool Graphics::LoadContent()
{
	//..........
	return true;
}

}

}