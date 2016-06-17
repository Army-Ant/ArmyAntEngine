#ifndef BASE_HPP_2015_11_12
#define BASE_HPP_2015_11_12

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#ifdef DIRECTX_11_3_USED_EXPORTS
#include <d3d11_4.h>
#include <d3dx11.h> 
#include <DxErr.h>
#include <d3dcompiler.h>
#include <xnamath.h>
template<class PTR>
inline void AA_SAFE_RELEASE(PTR&ptr) { if (ptr != nullptr)ptr->Release(); ptr = nullptr; }
#elif defined OPENGL_USED_EXPORTS

#elif defined OS_WINDOWS
#include "AA_D3D11_3_used.h"

#else

#endif


#include <ArmyAnt.h>

#endif // BASE_HPP_2015_11_12
