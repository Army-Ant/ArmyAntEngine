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

	//�趨��������
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//������
	wndClass.lpfnWndProc = WndProc;		//��Ϣ�ص�
	wndClass.hInstance = hInstance;		//���̾��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//���ָ��״̬
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//���ڱ���ɫ
	wndClass.lpszMenuName = NULL;		//�˵�
	wndClass.lpszClassName = "DX11BookWindowClass";		//ע������

	//ע�ᴰ��
	if(!RegisterClassExA(&wndClass))
		return -1;

	long xmid = GetSystemMetrics(SM_CXSCREEN) / 2;
	long ymid = GetSystemMetrics(SM_CYSCREEN) / 2;
	long rcw = 640;
	long rch = 480;
	RECT rc = {0,0,rcw,rch};	//����λ�ô�С
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	HWND hwnd = CreateWindowExA(0l, wndClass.lpszClassName, "Blank DirectX3D-11 Window", WS_OVERLAPPEDWINDOW, xmid - rcw / 2, ymid - rch / 2, rcw, rch, NULL, NULL, hInstance, NULL);
	if(!hwnd)
		return -1;
	ShowWindow(hwnd, nShow);

	//Demo Initialize  
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
		}
	}
	//Demo Shutdown  
	return static_cast<int>(msg.wParam);
}