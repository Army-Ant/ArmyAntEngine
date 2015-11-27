#include <windows.h>
#include "AA_D3D11_3_used.h"

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
	std::auto_ptr<AA_Engine::AA_D3dRef::Demo> demo(new AA_Engine::AA_D3dRef::Demo());
	// Demo Initialize    
	bool result = demo->Initialize( hInstance, hwnd );
	//Demo Initialize  
	/*auto ret = new AA_Engine::AA_D3dRef::D3dBase(hwnd);
	auto viewport = ret->CreateViewport();
	AA_Engine::Algorithm::XmFloat3 vertices[] =
	{
		AA_Engine::Algorithm::XmFloat3(0.5f, 0.5f, 0.5f),
		AA_Engine::Algorithm::XmFloat3(0.5f, -0.5f, 0.5f),
		AA_Engine::Algorithm::XmFloat3(-0.5f, -0.5f, 0.5f),
	};
	auto buffer = ret->MakeBuffer(AA_Engine::AA_D3dRef::BufferType::Vertex, sizeof(AA_Engine::Algorithm::XmFloat3) * 3, vertices);
	AAAssert(buffer != nullptr);
	AAAssert(buffer->CreateShader("D3D11Shader.hlsl", true, "VertexShaderMain"));
	AAAssert(buffer->CreateInputLayout(1, AA_Engine::Algorithm::Color32(0x0000ee7f)));
	AAAssert(buffer->CreateShader("D3D11Shader.hlsl", false, "PixelShaderMain"));
	AAAssert(buffer->CreateBuffer());
	ret->ResetViewport(0x000040ff);
	*/
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
			//AAAssert(buffer->Render());
			demo->Render( ); 
		}
	}
	//Demo Shutdown 
	/*ret->ReleaseBuffer(buffer);
	AA_SAFE_DEL(ret);*/
	demo->Shutdown();
	return static_cast<int>(msg.wParam);
}