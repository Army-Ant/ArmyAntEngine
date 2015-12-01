#include "base.hpp"
#include "D3dBase.h"
#include "../externals/ArmyAntLib/ArmyAnt.h"
using namespace ArmyAnt;

namespace AA_Engine {

namespace AA_D3dRef {


class D3dBase_private
{
public:
	D3dBase_private() {}
	~D3dBase_private() {}

	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	unsigned int width = 0;
	unsigned int height = 0;

	ID3D11RenderTargetView* backBuffer = nullptr;

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
	if(hd->swapChain)
		hd->swapChain->Release();
	if(hd->context)
		hd->context->Release();
	if(hd->device)
		hd->device->Release();
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
	hd->context->RSSetViewports(1, &viewport);
	return true;

}

bool D3dBase::ResetViewport(AA_Engine::Algorithm::Color32 color32/* = 0xffffffff*/)
{
	auto hd = handleManager.GetDataByHandle(handle);
	float clearColor[4] = {color32.simpleColor.red / 256.0f,color32.simpleColor.green / 256.0f,color32.simpleColor.blue / 256.0f,color32.simpleColor.alpha / 256.0f};
	hd->context->ClearRenderTargetView(hd->backBuffer, clearColor);
	return 0 <= hd->swapChain->Present(0, 0);
}


D3dBuffer* D3dBase::MakeBuffer(BufferType type, DWORD datalen, void*datas)
{
	auto ret = new D3dBuffer(*this);
	ret->SetType(type);
	ret->SetDatas(datalen, datas);
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
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags, featureLevels, totalFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &hd->swapChain, &hd->device, &featureLevel_, &hd->context);
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
	auto result = hd->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&bufferTexture);
	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to get the swap chain back buffer!");
		return false;
	}
	result = hd->device->CreateRenderTargetView(bufferTexture, 0, &hd->backBuffer);
	if(bufferTexture)
		AA_SAFE_RELEASE(bufferTexture);
	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to create the render target view!");  
		return false;
	}
	hd->context->OMSetRenderTargets(1, &hd->backBuffer, nullptr);
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

	ID3DBlob* vsBuffer = nullptr;
	ID3DBlob* psBuffer = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;

	ID3D11ShaderResourceView* resView; 
	ID3D11SamplerState* samplerState;

public:
	static D3D11_INPUT_ELEMENT_DESC GetInputDesc(const char* semanticName, DWORD semanticIndex, DXGI_FORMAT format, DWORD inputSlot, DWORD alignBytesOffset, BufferType type);
	static ID3DBlob* GetCompileHLSL(const char*shaderCodeFile, const char* EntryPoint, bool isVertexShader);

	AA_FORBID_COPY_CTOR(D3dBuffer_Private);
	AA_FORBID_ASSGN_OPR(D3dBuffer_Private);
};

D3D11_INPUT_ELEMENT_DESC D3dBuffer_Private::GetInputDesc(const char* semanticName, DWORD semanticIndex, DXGI_FORMAT format, DWORD inputSlot, DWORD alignBytesOffset, BufferType type)
{
	D3D11_INPUT_ELEMENT_DESC ret;
	ret.SemanticName = semanticName;
	ret.SemanticIndex = semanticIndex;
	ret.Format = format;
	ret.InputSlot = inputSlot;
	ret.AlignedByteOffset = alignBytesOffset;
	switch(type)
	{
		case BufferType::Vertex:
			ret.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			ret.InstanceDataStepRate = 0;
			break;
		default:
			ret.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	}
	return ret;
}

ID3DBlob* D3dBuffer_Private::GetCompileHLSL(const char*shaderCodeFile, const char* EntryPoint, bool isVertexShader)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG 
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* outBuffer = nullptr;
	ID3DBlob* errorBuffer = nullptr;
	HRESULT result;
	result = D3DX11CompileFromFile(shaderCodeFile, 0, 0, EntryPoint, isVertexShader ? "vs_4_0" : "ps_4_0", shaderFlags, 0, 0, &outBuffer, &errorBuffer, 0);
	if(0 > result)
	{
		if(errorBuffer != 0)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			AA_SAFE_RELEASE(errorBuffer)
		}
	}
	if(errorBuffer != 0)
		AA_SAFE_RELEASE(errorBuffer);
	return outBuffer;
}

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
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	AA_SAFE_RELEASE(hd->vsBuffer);
	AA_SAFE_RELEASE(hd->psBuffer);
	AA_SAFE_RELEASE(hd->vertexShader);
	AA_SAFE_RELEASE(hd->pixelShader);
	AA_SAFE_RELEASE(hd->inputLayout);
	AA_SAFE_RELEASE(hd->vertexBuffer);

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
	memset(hd->datas, 0, hd->datalen);
	hd->datas = datas;
	hd->datalen = datalen;
	return datas != nullptr && datalen > 0;
}

bool D3dBuffer::CreateBuffer()
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = hd->datalen;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = hd->datas;
	return (0 <= handleManager.GetDataByHandle(hd->parent->handle)->device->CreateBuffer(&bufferDesc, &data, &hd->vertexBuffer));
}

bool D3dBuffer::CreateShader(const char*shaderCodeFile, bool isVertexShader/* = true*/, const char* EntryPoint/* = nullptr*/)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	auto phd = handleManager.GetDataByHandle(hd->parent->handle);
	HRESULT result = -1;

	if(isVertexShader)
	{
		hd->vsBuffer = hd->GetCompileHLSL(shaderCodeFile, EntryPoint, true);
		result = phd->device->CreateVertexShader(hd->vsBuffer->GetBufferPointer(), hd->vsBuffer->GetBufferSize(), 0, &hd->vertexShader);
		if(FAILED(result))
		{
			if(hd->vsBuffer != nullptr)
				AA_SAFE_RELEASE(hd->vsBuffer)
				return false;
		}
	}
	else
	{
		hd->psBuffer = hd->GetCompileHLSL(shaderCodeFile, EntryPoint, false);
		result = phd->device->CreatePixelShader(hd->psBuffer->GetBufferPointer(), hd->psBuffer->GetBufferSize(), 0, &hd->pixelShader);
		if(FAILED(result))
		{
			if(hd->pixelShader != nullptr)
				AA_SAFE_RELEASE(hd->psBuffer)
				return false;
		}
	}
	return result >= 0;
}

bool D3dBuffer::ReleaseShader(bool isVertexShader)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	if(isVertexShader)
		AA_SAFE_RELEASE(hd->vsBuffer)
	else
		AA_SAFE_RELEASE(hd->psBuffer)
		return true;
}

bool D3dBuffer::CreateInputLayout(DWORD pointNums, AA_Engine::Algorithm::Color32 innerColor)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	auto phd = handleManager.GetDataByHandle(hd->parent->handle);
	D3D11_INPUT_ELEMENT_DESC layout;

	layout = hd->GetInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, hd->type);
	if(FAILED(phd->device->CreateInputLayout(&layout, 1, hd->vsBuffer->GetBufferPointer(), hd->vsBuffer->GetBufferSize(), &hd->inputLayout)))
		return false;
	AA_SAFE_RELEASE(hd->vsBuffer);
	// Set the input layout
	return true;
}

bool D3dBuffer::Render()
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	auto phd = handleManager.GetDataByHandle(hd->parent->handle);

	unsigned int stride = sizeof(_XMFLOAT3);
	unsigned int offset = 0;
	phd->context->IASetInputLayout(hd->inputLayout);
	phd->context->IASetVertexBuffers(0, 1, &hd->vertexBuffer, &stride, &offset);
	phd->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Render a triangle
	phd->context->VSSetShader(hd->vertexShader, nullptr, 0);
	phd->context->PSSetShader(hd->pixelShader, nullptr, 0);
	phd->context->Draw(3, 0);

	// Present the information rendered to the back buffer to the front buffer (the screen)
	return 0 <= phd->swapChain->Present(0, 0);
}

}

}