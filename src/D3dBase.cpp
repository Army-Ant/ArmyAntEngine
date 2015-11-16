#include "D3dBase.h"
#include "../externals/ArmyAntLib/ArmyAnt.h"

namespace AA_Engine {

namespace AA_D3dRef {

class D3dBase_private
{

};

static ArmyAnt::ClassPrivateHandleManager<D3dBase, D3dBase_private, unsigned int> handleManager;

D3dBase::D3dBase()
	:handle(handleManager.GetHandle(this))
{
}

D3dBase::~D3dBase()
{
	handleManager.ReleaseHandle(handle);
}

}

}