#ifndef A_A_D3D11_3_USED_H_2015_11_13
#define A_A_D3D11_3_USED_H_2015_11_13

#include "D3dBase.h"

#ifndef DIRECTX_11_3_USED_EXPORTS
#ifdef _32BIT

#ifdef NDEBUG
#pragma comment(lib,"DirectX_11_3_Used.lib")
#else	// NDEBUG
#pragma comment(lib,"DirectX_11_3_Usedd.lib")
#endif // NDEBUG

#else // _32BIT

#ifdef NDEBUG
#pragma comment(lib,"DirectX_11_3_Used64.lib")
#else	// NDEBUG
#pragma comment(lib,"DirectX_11_3_Usedd64.lib")
#endif // NDEBUG

#endif // ARMYANTLIB_EXPORTS
#endif // _32BIT

#endif // A_A_D3D11_3_USED_H_2015_11_13