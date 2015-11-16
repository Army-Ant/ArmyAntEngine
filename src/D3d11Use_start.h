#ifndef D3D11_USE_START_H_2015_11_13
#define D3D11_USE_START_H_2015_11_13

#ifdef _WIN32

#ifdef DIRECTX_11_3_USED_EXPORTS
#define D3D11USE_API __declspec(dllexport)
#else
#define D3D11USE_API __declspec(dllimport)
#endif

#endif // _WIN32

#endif // D3D11_USE_START_H_2015_11_13
