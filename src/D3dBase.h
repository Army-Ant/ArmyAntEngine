#ifndef D3D_BASE_H_2015_11_13
#define D3D_BASE_H_2015_11_13

#include "D3d11Use_start.h"
#include "../externals/ArmyAntLib/include/AADefine.h"

namespace AA_Engine {

namespace AA_D3dRef {

class D3D11USE_API D3dBase
{
public:
	D3dBase();
	virtual ~D3dBase();

public:
	bool CreateBuffer();
	bool CreateViewport();
	bool ResetViewport();

public:
	const unsigned int handle;

protected:
	bool CreateDevice();

	AA_FORBID_EQUAL_OPR(D3dBase)
	AA_FORBID_COPY_CTOR_WITHCONST(D3dBase, handle(0))
};

}

}

#endif