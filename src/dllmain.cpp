﻿#include "base.hpp"
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dxgi.lib")  
#pragma comment(lib, "d3d11.lib")  
#pragma comment(lib, "d3dx11.lib")  
#pragma comment(lib, "d3dx10.lib") 

BOOL WINAPI DllMain(_In_ HANDLE _HDllHandle, _In_ DWORD _Reason, _In_opt_ LPVOID _Reserved)
{
	UNREFERENCED_PARAMETER(_HDllHandle);
	UNREFERENCED_PARAMETER(_Reserved);
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
	return TRUE;
}