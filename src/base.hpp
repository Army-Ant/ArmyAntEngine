#ifndef BASE_HPP_2015_11_12
#define BASE_HPP_2015_11_12

#include <windows.h>
#include <d3d11_2.h>
#include <d3dx11.h> 
#include <DxErr.h>
#include <d3dcompiler.h>

#include <xnamath.h>

#define AA_SAFE_RELEASE(ptr) {if(ptr!=nullptr)ptr->Release();ptr=nullptr;}

#include "../externals/ArmyAntLib/ArmyAnt.h"

#endif // BASE_HPP_2015_11_12
