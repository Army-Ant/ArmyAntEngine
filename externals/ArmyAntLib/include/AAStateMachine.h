#ifndef AA_STATE_MACHINE_H_2015_12_2
#define AA_STATE_MACHINE_H_2015_12_2

/*	* @ author			: Jason
	* @ date			: 12/02/2015
	* @ nearly update	: 12/02/2015
	* @ small version	: 0.1
	* @ summary			: 状态机
	* @ uncompleted		: all
	* @ untested		: 
	* @ tested			: 
*/

#include "AADefine.h"
#include "AA_start.h"
#include <functional>

namespace ArmyAnt {

namespace StateMachineHelper {

typedef std::function<bool(const char*lastState, const char*nextState, void*params)> StateChangeFunc;
typedef std::function<bool(const char*thisState, DWORD delayTime, void*params)> StateUpdateFunc;

struct State
{
	char name[128] = "";
	StateChangeFunc OnEnter = nullptr;
	StateChangeFunc OnLeave = nullptr;
	StateUpdateFunc OnUpdate = nullptr;
};

}

class ARMYANTLIB_API StateMachine
{
public:
	StateMachine();
	StateMachine(const StateMachine&value);
	StateMachine&operator=(const StateMachine&value);
	~StateMachine();

public:
	bool InsertState(const StateMachineHelper::State&state, bool isDefaultState = false);
	bool DeleteState(const char*name);
	bool IsStateExist(const char*name)const;
	const char* GetAllStates()const;

	bool GoToState(const char*name = nullptr);
	bool AbortToState(const char*name = nullptr, StateMachineHelper::StateChangeFunc dealFunc = nullptr);
	const char* GetCurrentState()const;
	const char* GetDefaultState()const;
	bool SetDefaultState(const char*name);
	bool EnableState(const char*name);
	bool IsStateEnable(const char*name)const;

	bool Start(const char*name = nullptr);
	bool Pause();
	bool Resume();
	bool IsRunning()const;

public:
	StateMachine& operator+=(StateMachineHelper::State&state);
	StateMachine& operator-=(StateMachineHelper::State&state);
	StateMachineHelper::State& operator[](const char*name);
	const StateMachineHelper::State& operator[](const char*name)const;
	StateMachine& operator>>(const char*name);

public:
	const DWORD handle;
};

}

#endif // AA_STATE_MACHINE_H_2015_12_2