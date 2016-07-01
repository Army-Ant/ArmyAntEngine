#include "base.hpp"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	switch(message)
	{
		case WM_PAINT:
			hDC = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR cmdLine, _In_ int nShow)
{
	WNDCLASSEX wndClass = {0};
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	//设定窗口属性
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//窗体风格
	wndClass.lpfnWndProc = WndProc;		//消息回调
	wndClass.hInstance = hInstance;		//进程句柄
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//鼠标指针状态
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//窗口背景色
	wndClass.lpszMenuName = NULL;		//菜单
	wndClass.lpszClassName = "DX11BookWindowClass";		//注册类名

	//注册窗口
	if(!RegisterClassExA(&wndClass))
		return -1;

	long xmid = GetSystemMetrics(SM_CXSCREEN) / 2;
	long ymid = GetSystemMetrics(SM_CYSCREEN) / 2;
	long rcw = 640;
	long rch = 480;
	RECT rc = {0,0,rcw,rch};	//窗口位置大小
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	HWND hwnd = CreateWindowExA(0l, wndClass.lpszClassName, "Blank DirectX3D-11 Window", WS_OVERLAPPEDWINDOW, xmid - rcw / 2, ymid - rch / 2, rcw, rch, NULL, NULL, hInstance, NULL);
	if(!hwnd)
		return -1;
	ShowWindow(hwnd, nShow);


	RECT dimensions;
	GetClientRect(hwnd, &dimensions);
	unsigned int width = dimensions.right - dimensions.left;
	unsigned int height = dimensions.bottom - dimensions.top;

	struct VertexData {
		AA_Engine::Algorithm::XmFloat3 f;
		AA_Engine::Algorithm::XmFloat4 c;
	};

	struct Texture2DData {
		AA_Engine::Algorithm::XmFloat3 f;
		AA_Engine::Algorithm::XmFloat2 d;
	};

	auto ret = new AA_Engine::AA_D3dRef::D3dBase(hwnd, 1, 1, 0, 60, width, height);
	auto cam = AA_Engine::AA_D3dRef::D3dCamera();
	cam.SetPosition(0.0f, 0.0f, -10.0f);
	AAAssert(ret->CreateViewport(0.1f, 1000.f), -1);
	AAAssert(ret->ResetViewport(AA_Engine::Algorithm::Color32(0, 0, 127, 255)), -1);
	uint32 indices[] = { 0,1,2 };
	VertexData vertices[] =
	{
		{AA_Engine::Algorithm::XmFloat3(-0.9f, -0.9f, 0.0f),AA_Engine::Algorithm::XmFloat4(0.0f, 1.0f, 0.0f, 1.0f)},
		{AA_Engine::Algorithm::XmFloat3(0.0f, 0.9f, 0.0f),AA_Engine::Algorithm::XmFloat4(0.0f, 1.0f, 0.0f, 1.0f) },
		{AA_Engine::Algorithm::XmFloat3(0.9f, -0.9f, 0.0f),AA_Engine::Algorithm::XmFloat4(0.0f, 1.0f, 0.0f, 1.0f) }
	};
	Texture2DData textureFrame[] =
	{
		{ AA_Engine::Algorithm::XmFloat3(1.f, 1.f, 1.f),AA_Engine::Algorithm::XmFloat2(1, 1) },
		{ AA_Engine::Algorithm::XmFloat3(1.f, -1.f, 1.f),AA_Engine::Algorithm::XmFloat2(1, 1) },
		{ AA_Engine::Algorithm::XmFloat3(-1.f, -1.f, 1.f),AA_Engine::Algorithm::XmFloat2(1, 1) },
		{ AA_Engine::Algorithm::XmFloat3(-1.f, -1.f, 1.f),AA_Engine::Algorithm::XmFloat2(1, 1) },
		{ AA_Engine::Algorithm::XmFloat3(-1.f, 1.f, 1.f),AA_Engine::Algorithm::XmFloat2(1, 1) },
		{ AA_Engine::Algorithm::XmFloat3(1.f, 1.f, 1.f),AA_Engine::Algorithm::XmFloat2(1, 1) }
	};
	auto buffer = ret->MakeBuffer();
	auto tex = ret->CreateTexture("101-CF_Lava01.png", AA_Engine::AA_D3dRef::TextureFileType::PNG);
#if 1
	AAAssert(buffer->SetVertexData(3, sizeof(VertexData) * 3, vertices), -2);
	AAAssert(buffer->SetIndexData(indices, sizeof(uint32) * 3), -2);
	AAAssert(buffer->CreateShader("D3D11Shader.hlsl", true, "VertexShaderMain_Triangle"),-3);
	AAAssert(buffer->CreateShader("D3D11Shader.hlsl", false, "PixelShaderMain_Triangle"),-5);
	AAAssert(buffer->CreateInputLayout(AA_Engine::AA_D3dRef::DataType::Float3, AA_Engine::AA_D3dRef::DataType::Float4, AA_Engine::AA_D3dRef::DataType::Null), -4);
#else
	AAAssert(buffer->SetVertexData(6, sizeof(Texture2DData) * 6, textureFrame), -2);
	AAAssert(buffer->SetTextureData(*tex),-2);
	AAAssert(buffer->CreateShader("D3D11Shader.hlsl", true, "VertexShaderMain_Texture"), -3);
	AAAssert(buffer->CreateInputLayout(AA_Engine::AA_D3dRef::DataType::Float3, AA_Engine::AA_D3dRef::DataType::Float2), -4);
	AAAssert(buffer->CreateShader("D3D11Shader.hlsl", false, "PixelShaderMain_Texture"), -5);
	AAAssert(buffer->CreateView(AA_Engine::AA_D3dRef::DataType::Null), -5);
#endif
	ret->ResetViewport(0xffffffff);
	
	MSG msg = {0};
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Update 
			//Draw     
			AAAssert(buffer->Render(cam),-7);
		}
	}
	//Demo Shutdown 
	ret->ReleaseBuffer(buffer);
	ret->RemoveTexture(tex);
	ArmyAnt::Fragment::AA_SAFE_DEL(ret);
	return static_cast<int>(msg.wParam);
}