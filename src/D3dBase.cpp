#include "base.hpp"
#include "D3dBase.h"
#include <ArmyAnt.h>
#include <d3dx9math.h>
using namespace ArmyAnt;

namespace AA_Engine {

namespace AA_D3dRef {

	
/******************* Private inner structure definations *****************************************/

struct MatrixBufferType
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

class D3dBase_private
{
public:
	D3dBase_private() {}
	~D3dBase_private()
	{
		if (depthStencilBuffer)
			depthStencilBuffer->Release();
		if (depthStencilState)
			depthStencilState->Release();
		if (depthStencilView)
			depthStencilView->Release();
		if (backBuffer)
			backBuffer->Release();
		if (swapChain)
			swapChain->Release();
		if (context)
			context->Release();
		if (device)
			device->Release();
	}

	uint32 width = 0;
	uint32 height = 0;
	uint32 fps = 60;
	float screenNear = 0;
	float screenDepth = 0;
	ID3D11Texture2D* depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11RenderTargetView* backBuffer = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11DeviceContext* context = nullptr;
	ID3D11Device* device = nullptr;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX worldMatrix; 
	D3DXMATRIX orthoMatrix;
	AA_FORBID_COPY_CTOR(D3dBase_private);
	AA_FORBID_ASSGN_OPR(D3dBase_private);
};

class D3dBuffer_Private
{
public:
	D3dBuffer_Private(const D3dBase*parent) :parent(parent) {};
	~D3dBuffer_Private()
	{
		AA_SAFE_RELEASE(vsBuffer);
		AA_SAFE_RELEASE(psBuffer);
		AA_SAFE_RELEASE(vertexShader);
		AA_SAFE_RELEASE(pixelShader);
		AA_SAFE_RELEASE(inputLayout);
		AA_SAFE_RELEASE(vertexBuffer);
		AA_SAFE_RELEASE(indexBuffer);
		AA_SAFE_RELEASE(matrixBuffer);
		AA_SAFE_RELEASE(textureView);
		AA_SAFE_RELEASE(samplerState);
		Fragment::AA_SAFE_DELALL(datas);
	};

public:
	const D3dBase* parent = nullptr;
	BufferType type = BufferType::Vertex;
	void*datas = nullptr;
	std::vector<uint32>indices;
	const Texture2D* texture = nullptr;
	uint32 vertexCount = 0;
	uint32 datalen = 0;
	uint32 totalStride = 0;

	ID3DBlob* vsBuffer = nullptr;
	ID3DBlob* psBuffer = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* matrixBuffer = nullptr;
	ID3D11ShaderResourceView* textureView = nullptr;

	ID3D11SamplerState* samplerState = nullptr;

public:
	bool CreateBuffer(BufferType t);

	static D3D11_INPUT_ELEMENT_DESC GetInputDesc(const char* semanticName, DWORD semanticIndex, DXGI_FORMAT format, DWORD inputSlot, DWORD alignBytesOffset, BufferType type);
	static ID3DBlob* GetCompileHLSL(const char*shaderCodeFile, const char* EntryPoint, bool isVertexShader);
	static DXGI_FORMAT GetFormatByType(DataType type);
	static uint32 GetSizeOFfset(DataType type);

	AA_FORBID_COPY_CTOR(D3dBuffer_Private);
	AA_FORBID_ASSGN_OPR(D3dBuffer_Private);
};

class Texture2D_Private
{
public:
	Texture2D_Private(const D3dBase* parent) :parent(parent) {};
	~Texture2D_Private()
	{
		AA_SAFE_RELEASE(data);
	};

public:
	const D3dBase* parent = nullptr;
	std::string filename = "";
	TextureFileType type = TextureFileType::Null;
	ID3D11Resource* data = nullptr;

	AA_FORBID_COPY_CTOR(Texture2D_Private);
	AA_FORBID_ASSGN_OPR(Texture2D_Private);
};

class D3dCamera_Private
{
public:
	D3dCamera_Private() {};
	D3dCamera_Private(const D3dCamera_Private&value);
	D3dCamera_Private& operator=(const D3dCamera_Private&value);
	~D3dCamera_Private() {};

public:
	D3DXMATRIX viewMatrix;
	Algorithm::XmFloat3 position;
	Algorithm::XmFloat3 rotation;
	
	D3DXMATRIX& GetViewMatrix();
};

static ArmyAnt::ClassPrivateHandleManager<D3dBase, D3dBase_private, uint32> handleManager;
static ArmyAnt::ClassPrivateHandleManager<D3dBuffer, D3dBuffer_Private, uint32> bufferHandleManager;
static ArmyAnt::ClassPrivateHandleManager<Texture2D, Texture2D_Private, uint32> textureHandleManager;
static ArmyAnt::ClassPrivateHandleManager<D3dCamera, D3dCamera_Private, uint32> cameraHandleManager;

/************************** Source Code : D3dBase ***********************************************/

D3dBase::D3dBase(HWND window, DWORD bufferCount /* = 1 */, DWORD SampleDescCount /* = 1 */, DWORD SampleDescQuality /* = 0 */, DWORD fps /* = 60 */, DWORD width /* = 0 */, DWORD height /* = 0 */)
	:handle(handleManager.GetHandle(this))
{
	AAAssert(CreateDevice(window, bufferCount, SampleDescCount, SampleDescQuality, fps, width, height),);
	AAAssert(CreateBackBuffer(),);
}

D3dBase::~D3dBase()
{
	AAAssert(ReleaseBackBuffer(),);
	handleManager.ReleaseHandle(handle);
}

bool D3dBase::CreateViewport(float screenNear, float screenDepth)
{
	auto hd = handleManager.GetDataByHandle(handle);
	return CreateViewport(0.0f, 0.0f, static_cast<float>(hd->width), static_cast<float>(hd->height), 0.0f, 1.0f, screenNear, screenDepth);
}


bool D3dBase::CreateViewport(float x, float y, float w, float h, float minDepth, float maxDepth, float screenNear, float screenDepth)
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
	hd->screenNear = screenNear;
	hd->screenDepth = screenDepth;
	D3DXMatrixPerspectiveFovLH(&hd->projectionMatrix, D3DX_PI / 4.0f, w / h, screenNear, screenDepth);
	D3DXMatrixIdentity(&hd->worldMatrix);
	D3DXMatrixOrthoLH(&hd->orthoMatrix, w, h, screenNear, screenDepth);
	return true;
}

bool D3dBase::ResetViewport(AA_Engine::Algorithm::Color32 color32/* = 0xffffffff*/)
{
	auto hd = handleManager.GetDataByHandle(handle);
	float clearColor[4] = {color32.simpleColor.red / 256.0f,color32.simpleColor.green / 256.0f,color32.simpleColor.blue / 256.0f,color32.simpleColor.alpha / 256.0f};
	hd->context->ClearRenderTargetView(hd->backBuffer, clearColor);
	// Clear the depth buffer.  
	hd->context->ClearDepthStencilView(hd->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	return 0 <= hd->swapChain->Present(0, 0);
}

D3dBuffer* D3dBase::MakeBuffer()
{
	auto ret = new D3dBuffer(*this);
	return ret;
}

bool D3dBase::ReleaseBuffer(D3dBuffer*buffer)
{
	Fragment::AA_SAFE_DEL(buffer);
	return buffer==nullptr;
}

Texture2D* D3dBase::CreateTexture()
{
	auto ret = new Texture2D(*this);
	return ret;
}

Texture2D* D3dBase::CreateTexture(const char*filename, TextureFileType type)
{
	auto ret = new Texture2D(*this, filename, type);
	return ret;
}

bool D3dBase::RemoveTexture(Texture2D*texture)
{
	Fragment::AA_SAFE_DEL(texture);
	return texture == nullptr;
}

const DWORD D3dBase::GetScreenWidth()
{
#ifdef OS_WINDOWS
	return GetSystemMetrics(SM_CXSCREEN);
#else

#endif
}


const DWORD D3dBase::GetScreenHeight()
{
#ifdef OS_WINDOWS
	return GetSystemMetrics(SM_CYSCREEN);
#else

#endif
}

bool D3dBase::CreateDevice(HWND window, DWORD bufferCount, DWORD SampleDescCount, DWORD SampleDescQuality, DWORD fps, DWORD width, DWORD height, const int* sparedSoftware/* = nullptr*/)
{
	auto hd = handleManager.GetDataByHandle(handle);

	hd->width = width;
	hd->height = height;
	hd->fps = fps;
	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE };
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0,D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_10_0 };
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = hd->width;		//缓存页宽度
	swapChainDesc.BufferDesc.Height = hd->height;	//缓存页高度
	swapChainDesc.BufferDesc.RefreshRate.Numerator = hd->fps;		//每周期刷新次数
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;		//周期长度(秒)
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//缓存格式
	swapChainDesc.SampleDesc.Count = SampleDescCount;		//取样数量
	swapChainDesc.SampleDesc.Quality = SampleDescQuality;	//取样质量
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//缓存用法
	swapChainDesc.BufferCount = bufferCount;		//缓存页面总数
	swapChainDesc.OutputWindow = window;		//窗口句柄
	swapChainDesc.Windowed = TRUE;		//全屏模式中，是否继续使用原先的尺寸
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags;

	unsigned int creationFlags = 0;
#ifdef _DEBUG   
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif  
	HRESULT result = -1;
	for (unsigned int driver = 0; driver < totalDriverTypes; ++driver)
	{
		HMODULE s = nullptr;
		if (driverTypes[driver] == D3D_DRIVER_TYPE_SOFTWARE || sparedSoftware != nullptr)
			s = (HMODULE)sparedSoftware;
		D3D_FEATURE_LEVEL usedLevel = D3D_FEATURE_LEVEL_12_1;
		result = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[driver], s, creationFlags, featureLevels, totalFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &hd->swapChain, &hd->device, &usedLevel, &hd->context);
		if (SUCCEEDED(result))
		{
			return usedLevel >= 0;
		}
	}
	return result >= 0 && hd->swapChain != nullptr;
}

bool D3dBase::CreateBackBuffer()
{
	auto hd = handleManager.GetDataByHandle(handle);
	ID3D11Texture2D* bufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	auto result = hd->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&bufferTexture);
	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to get the swap chain back buffer!");
		return false;
	}
	result = hd->device->CreateRenderTargetView(bufferTexture, 0, &hd->backBuffer);
	AA_SAFE_RELEASE(bufferTexture);
	if(FAILED(result))
	{
		//DXTRACE_MSG("Failed to create the render target view!");  
		return false;
	}

	// Initialize the description of the depth buffer.  
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.  
	depthBufferDesc.Width = hd->width;
	depthBufferDesc.Height = hd->height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	// Create the texture for the depth buffer using the filled out description.  
	result = hd->device->CreateTexture2D(&depthBufferDesc, NULL, &hd->depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// Initialize the description of the stencil state.  
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.  
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.  
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.  
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Create the depth stencil state.  
	result = hd->device->CreateDepthStencilState(&depthStencilDesc, &hd->depthStencilState);
	if (FAILED(result))
	{
		return false;
	}
	// Set the depth stencil state.  
	hd->context->OMSetDepthStencilState(hd->depthStencilState, 1);
	// Initailze the depth stencil view.  
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.  
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.  
	result = hd->device->CreateDepthStencilView(hd->depthStencilBuffer, &depthStencilViewDesc, &hd->depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	hd->context->OMSetRenderTargets(1, &hd->backBuffer, hd->depthStencilView);
	return true;
}

bool D3dBase::ReleaseBackBuffer()
{
	auto hd = handleManager.GetDataByHandle(handle);
	return  (AA_SAFE_RELEASE(hd->backBuffer), true) && (AA_SAFE_RELEASE(hd->depthStencilView), true);
}


/************************** Source Code : D3dBuffer ***********************************************/


bool D3dBuffer_Private::CreateBuffer(BufferType t)
{
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA data;
	D3D11_SAMPLER_DESC colorMapDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	ZeroMemory(&data, sizeof(data));
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
	type = t;

	switch (type)
	{
	case BufferType::Texture2D:
		colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;
		return 0 <= handleManager.GetDataByHandle(parent->handle)->device->CreateSamplerState(&colorMapDesc, &samplerState);
	case BufferType::Vertex:
		bufferDesc.ByteWidth = datalen;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.StructureByteStride = 0;
		data.pSysMem = datas;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;
		return 0 <= handleManager.GetDataByHandle(parent->handle)->device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	case BufferType::Index:
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(unsigned long) * indices.size();
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		data.pSysMem = &(indices[0]);
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		// Create the index buffer.
		return 0 <= handleManager.GetDataByHandle(parent->handle)->device->CreateBuffer(&bufferDesc, &data, &indexBuffer);
	case BufferType::Matrix:
		bufferDesc.ByteWidth = sizeof(MatrixBufferType);
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.StructureByteStride = 0;
		return 0 <= handleManager.GetDataByHandle(parent->handle)->device->CreateBuffer(&bufferDesc, nullptr, &matrixBuffer);
	default:
		return false;
	}
}

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
			break;
		default:
			ret.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	}
	ret.InstanceDataStepRate = 0;
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
	result = D3DX11CompileFromFile(shaderCodeFile, 0, 0, EntryPoint, isVertexShader ? "vs_5_0" : "ps_5_0", shaderFlags, 0, 0, &outBuffer, &errorBuffer, 0);
	if(0 > result)
	{
		if(errorBuffer != 0)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			AA_SAFE_RELEASE(errorBuffer);
		}
	}
	if(errorBuffer != 0)
		AA_SAFE_RELEASE(errorBuffer);
	return outBuffer;
}

DXGI_FORMAT D3dBuffer_Private::GetFormatByType(DataType type)
{
	switch (type)
	{
	case DataType::Float2:
		return DXGI_FORMAT_R32G32_FLOAT;
	case DataType::Float3:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case DataType::Float4:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case DataType::Integer2:
		return DXGI_FORMAT_R32G32_SINT;
	case DataType::Integer3:
		return DXGI_FORMAT_R32G32B32_SINT;
	case DataType::Integer4:
		return DXGI_FORMAT_R32G32B32A32_SINT;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

uint32 D3dBuffer_Private::GetSizeOFfset(DataType type)
{
	switch (type)
	{
	case DataType::Float2:
		return sizeof(Algorithm::XmFloat2);
	case DataType::Float3:
		return sizeof(Algorithm::XmFloat3);
	case DataType::Float4:
		return sizeof(Algorithm::XmFloat4);
	case DataType::Integer2:
		return sizeof(Algorithm::XmFloat2);
	case DataType::Integer3:
		return sizeof(Algorithm::XmFloat3);
	default:
		return 0;
	}
}

D3dBuffer::D3dBuffer(const D3dBase&parent)
	:handle(bufferHandleManager.GetHandle(this, new D3dBuffer_Private(&parent)))
{
}

D3dBuffer::~D3dBuffer()
{
	bufferHandleManager.ReleaseHandle(handle);
}

bool D3dBuffer::SetVertexData(uint32 vertexCount, uint32 datalen, void*datas)
{
	if (datas == nullptr && datalen <= 0)
		return false;
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	Fragment::AA_SAFE_DELALL(hd->datas);
	hd->datas = new uint8[datalen];
	hd->vertexCount = vertexCount;
	hd->datalen = datalen;
	memcpy(hd->datas, datas, datalen);
	return hd->CreateBuffer(BufferType::Vertex);
}

bool D3dBuffer::SetIndexData(const uint32* indices, uint32 datalen)
{
	if (indices == nullptr && datalen <= 0)
		return false;
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	hd->indices.clear();
	for (uint32 i = 0; i < datalen; i++) {
		hd->indices.push_back(indices[i]);
	}
	return hd->CreateBuffer(BufferType::Index);
}

bool D3dBuffer::SetTextureData(const Texture2D&datas)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	hd->texture = &datas;
	return hd->CreateBuffer(BufferType::Texture2D);
}

bool D3dBuffer::CreateShader(const char*shaderCodeFile, bool isVertexShader, const char* EntryPoint/* = nullptr*/)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	auto phd = handleManager.GetDataByHandle(hd->parent->handle);
	HRESULT result = -1;

	if (isVertexShader)
	{
		hd->vsBuffer = hd->GetCompileHLSL(shaderCodeFile, EntryPoint, true);
		result = phd->device->CreateVertexShader(hd->vsBuffer->GetBufferPointer(), hd->vsBuffer->GetBufferSize(), 0, &hd->vertexShader);
	}
	else
	{
		hd->psBuffer = hd->GetCompileHLSL(shaderCodeFile, EntryPoint, false);
		result = phd->device->CreatePixelShader(hd->psBuffer->GetBufferPointer(), hd->psBuffer->GetBufferSize(), 0, &hd->pixelShader);
	}
	if (FAILED(result))
	{
		if (hd->pixelShader != nullptr)
			AA_SAFE_RELEASE(hd->psBuffer);
		return false;
	}
	return result >= 0;
}

bool D3dBuffer::ReleaseShader(bool isVertexShader)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	if (isVertexShader)
		AA_SAFE_RELEASE(hd->vsBuffer);
	else
		AA_SAFE_RELEASE(hd->psBuffer);
	return true;
}

bool D3dBuffer::CreateView(DataType type)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	auto phd = handleManager.GetDataByHandle(hd->parent->handle);
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = hd->GetFormatByType(type);
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.MipLevels = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	auto result = phd->device->CreateShaderResourceView(textureHandleManager[hd->texture->handle]->data, &desc, &hd->textureView);
	return result >= 0;
}

bool D3dBuffer::CreateInputLayout(DataType posType, DataType colorType, DataType texcoordType)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	auto phd = handleManager.GetDataByHandle(hd->parent->handle);
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
	layout.push_back(hd->GetInputDesc("POSITION", 0, hd->GetFormatByType(posType), 0, 0, hd->type));
	if (colorType != DataType::Null)
	{
		layout.push_back(hd->GetInputDesc("COLOR", 0, hd->GetFormatByType(colorType), 0, D3D11_APPEND_ALIGNED_ELEMENT, hd->type));
	}
	if (texcoordType != DataType::Null)
	{
		layout.push_back(hd->GetInputDesc("TEXCOORD", 0, hd->GetFormatByType(texcoordType), 0, hd->GetSizeOFfset(posType), hd->type));
	}
	hd->totalStride = hd->GetSizeOFfset(posType) + hd->GetSizeOFfset(colorType) + hd->GetSizeOFfset(texcoordType);
	if(FAILED(phd->device->CreateInputLayout(&(layout[0]), layout.size(), hd->vsBuffer->GetBufferPointer(), hd->vsBuffer->GetBufferSize(), &hd->inputLayout)))
		return false;
	AA_SAFE_RELEASE(hd->vsBuffer);
	AA_SAFE_RELEASE(hd->psBuffer);
	// Set the input layout
	return hd->CreateBuffer(BufferType::Matrix);
}

bool D3dBuffer::Render(const D3dCamera&camera)
{
	auto hd = bufferHandleManager.GetDataByHandle(handle);
	auto phd = handleManager.GetDataByHandle(hd->parent->handle);

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	auto viewMatrix = cameraHandleManager[camera.handle]->GetViewMatrix();

	//一定要在发送给shader前转置这些矩阵，这是DirectX11的要求。
	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&phd->worldMatrix, &phd->worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&phd->projectionMatrix, &phd->projectionMatrix);
	//锁定m_matrixBuffer，设置新的matrices，然后解锁。

	// Lock the constant buffer so it can be written to.
	result = phd->context->Map(hd->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = phd->worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = phd->projectionMatrix;

	// Unlock the constant buffer.
	phd->context->Unmap(hd->matrixBuffer, 0);
	//现在更新HLSL vertex shader里面的matrix buffer。

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	phd->context->VSSetConstantBuffers(bufferNumber, 1, &hd->matrixBuffer);

	unsigned int stride = hd->totalStride;
	unsigned int offset = 0;
	phd->context->IASetVertexBuffers(0, 1, &hd->vertexBuffer, &stride, &offset);
	phd->context->IASetIndexBuffer(hd->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	phd->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	phd->context->IASetInputLayout(hd->inputLayout);
	// Render a triangle
	phd->context->VSSetShader(hd->vertexShader, nullptr, 0);
	phd->context->PSSetShader(hd->pixelShader, nullptr, 0);
	switch (hd->type)
	{
	case BufferType::Vertex:
		break;
	case BufferType::Texture2D:
		phd->context->PSSetShaderResources(0, 1, &hd->textureView);
		phd->context->PSSetSamplers(0, 1, &hd->samplerState);
	}

	phd->context->DrawIndexed(hd->indices.size(), 0, 0);
	// Present the information rendered to the back buffer to the front buffer (the screen)
	return 0 <= phd->swapChain->Present(0, 0);
}

D3dBase& D3dBuffer::GetDevice()
{
	return *const_cast<D3dBase*>(bufferHandleManager[handle]->parent);
}

const D3dBase& D3dBuffer::GetDevice() const
{
	return *bufferHandleManager[handle]->parent;
}


/************************** Source Code : D3dBuffer ***********************************************/

Texture2D::Texture2D(const D3dBase&parent)
	:handle(textureHandleManager.GetHandle(this, new Texture2D_Private(&parent)))
{

}

Texture2D::Texture2D(const D3dBase&parent, const char*filename, TextureFileType type)
	: handle(textureHandleManager.GetHandle(this, new Texture2D_Private(&parent)))
{
	AAAssert(LoadFromFile(filename, type),);
}

Texture2D::Texture2D(const Texture2D&value)
	: handle(textureHandleManager.GetHandle(this, new Texture2D_Private(textureHandleManager[value.handle]->parent)))
{
	*this = value;
}

Texture2D& Texture2D::operator=(const Texture2D&value)
{
	auto hd = textureHandleManager.GetDataByHandle(handle);
	auto vhd = textureHandleManager.GetDataByHandle(value.handle);
	hd->filename = vhd->filename;
	hd->parent = vhd->parent;
	hd->type = vhd->type;
	AA_SAFE_RELEASE(hd->data);
	hd->data = vhd->data;
	hd->data->AddRef();
	return *this;
}

Texture2D::~Texture2D()
{
	textureHandleManager.ReleaseHandle(handle);
}

bool Texture2D::LoadFromFile(const char*filename, TextureFileType type)
{
	//TODO: Test this function
	auto hd = textureHandleManager.GetDataByHandle(handle);
	D3DX11_IMAGE_LOAD_INFO info;
	D3DX11_IMAGE_INFO imgInfo;
	ZeroMemory(&info, sizeof(D3DX11_IMAGE_LOAD_INFO));
	ZeroMemory(&imgInfo, sizeof(D3DX11_IMAGE_INFO));
	info.Width;
	info.Height;
	info.Depth;
	info.FirstMipLevel;
	info.MipLevels;
	info.Usage;
	info.BindFlags;
	info.CpuAccessFlags;
	info.MiscFlags;
	info.Format;
	info.Filter;
	info.MipFilter;
	info.pSrcInfo = &imgInfo;
	imgInfo.Width;
	imgInfo.Height;
	imgInfo.Depth;
	imgInfo.ArraySize;
	imgInfo.MipLevels;
	imgInfo.MiscFlags;
	imgInfo.Format;
	imgInfo.ResourceDimension = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
	switch (type)
	{
	case TextureFileType::BMP:
		imgInfo.ImageFileFormat = D3DX11_IFF_BMP;
		break;
	case TextureFileType::JPEG:
		imgInfo.ImageFileFormat = D3DX11_IFF_JPG;
		break;
	case TextureFileType::GIF:
		imgInfo.ImageFileFormat = D3DX11_IFF_GIF;
		break;
	case TextureFileType::TIFF:
		imgInfo.ImageFileFormat = D3DX11_IFF_TIFF;
		break;
	case TextureFileType::DDS:
		imgInfo.ImageFileFormat = D3DX11_IFF_DDS;
		break;
	case TextureFileType::WMP:
		imgInfo.ImageFileFormat = D3DX11_IFF_WMP;
		break;
	case TextureFileType::PNG:
		imgInfo.ImageFileFormat = D3DX11_IFF_PNG;
		break;
	}
	auto ret = D3DX11CreateTextureFromFileA(handleManager[hd->parent->handle]->device, filename, nullptr, nullptr, &hd->data, nullptr);
	if (0 <= ret) {
		hd->filename = filename;
		hd->type = type;
		return true;
	}
	return false;
}


/************************** Source Code : D3dCamera ***********************************************/

D3dCamera_Private::D3dCamera_Private(const D3dCamera_Private&value)
{
	*this = value;
}

D3dCamera_Private& D3dCamera_Private::operator=(const D3dCamera_Private&value)
{
	position = value.position;
	rotation = value.rotation;
	viewMatrix = value.viewMatrix;
	return *this;
}

D3DXMATRIX& D3dCamera_Private::GetViewMatrix()
{
	D3DXVECTOR3 up, pos, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = pos + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &pos, &lookAt, &up);

	return viewMatrix;
}

D3dCamera::D3dCamera()
	:handle(cameraHandleManager.GetHandle(this,new D3dCamera_Private()))
{

}

D3dCamera::D3dCamera(const D3dCamera&value)
	: handle(cameraHandleManager.GetHandle(this, new D3dCamera_Private(*cameraHandleManager[value.handle])))
{

}

D3dCamera& D3dCamera::operator=(const D3dCamera&value)
{
	*cameraHandleManager[handle] = *cameraHandleManager[value.handle];
	return *this;
}

D3dCamera::~D3dCamera()
{
	cameraHandleManager.ReleaseHandle(handle);
}

void D3dCamera::SetPosition(float x, float y, float z)
{
	cameraHandleManager[handle]->position.Set(x, y, z);
}

void D3dCamera::SetPosition(Algorithm::XmFloat3 pos)
{
	SetPosition(pos.x, pos.y, pos.z);
}

void D3dCamera::SetRotation(float x, float y, float z)
{
	cameraHandleManager[handle]->rotation.Set(x, y, z);
}

void D3dCamera::SetRotation(Algorithm::XmFloat3 rot)
{
	SetRotation(rot.x, rot.y, rot.z);
}

Algorithm::XmFloat3 D3dCamera::GetPosition() const
{
	return cameraHandleManager[handle]->position;
}

Algorithm::XmFloat3 D3dCamera::GetRotation() const
{
	return cameraHandleManager[handle]->rotation;
}

}

}