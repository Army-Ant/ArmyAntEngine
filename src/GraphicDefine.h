#ifndef _GRAPHIC_DEFINE_H_2015_11_13
#define _GRAPHIC_DEFINE_H_2015_11_13

#include "D3d11Use_start.h"
#include "../ArmyAntLib/include/AADefine.h"
#include <string>

namespace AA_Engine {

namespace AA_Graphic {//================================================================
//Useful macros
//================================================================
//Safely release pointer referenced item
#define SAFF_RELEASE(ptr) {if(ptr){ptr->Release();(ptr)=nullptr;}}

#define SAFF_ON_LOST_DEVICE(ptr){if(ptr){ptr->onLostDevice();}}

#define SAFF_ON_RESET_DEVICE(ptr){if(ptr){ptr->onResetDevice();}}

#define TRANSCOLOR SETCOLOR_ARGB(0,255,0,255) //transparency color (magenta)

//================================================================================
//window
//================================================================================
const char CLASS_NAME[] = "ArmyAnt_what_ever";
const char GAME_TITLE[] = "ArmyAnt_GameI";
const bool FULLSCREEN = false;
const DWORD GAME_WIDTH = 1024;
const DWORD GAME_HEIGHT = 650;
//================================================================================
//game proporties
//================================================================================
const float PI = 3.14159265;
const float FRAME_RATE = 200.0f;
const float MIN_FRAME_RATE = 10.0f;
const float MIN_FRAME_TIME = 1.0 / FRAME_RATE;
const float MAX_FRAME_TIME = 1.0 / MIN_FRAME_RATE;
//================================================================================
//key mappings
//================================================================================
const DWORD ESC_KEY = VK_ESCAPE;
const DWORD WARRIOR_LEFT_KEY = VK_LEFT;
const DWORD WARRIOR_UP_KEY = VK_UP;
const DWORD WARRIOR_DOWN_KEY = VK_DOWN;
const DWORD WARRIOR_RIGHT_KEY = VK_RIGHT;
//================================================================================
//graphic images
//================================================================================


//================================================================================
//
//================================================================================


struct Animation
{
	std::string name;
	int numFrames;
	int x, y, w, h;
	float fps;
};

//Game Error class.Thrown when an error is detected by the game engine.
//Inherits for std::exception
class GameError : public std::exception
{
public:
	//default constructor
	GameError()throw() :errorCode(FATAL_ERROR), message("Undefined Error in game.") {}
	//copy constructor
	GameError(const GameError& e) throw() : std::exception(e), errorCode(e.errorCode), message(e.message) {}
	//constructor with args
	GameError(int code, const std::string &s) throw() :errorCode(code), message(s) {}
	//assignmetn operator
	GameError& operator= (const GameError& rhs) throw()
	{
		std::exception::operator=(rhs);
		this->errorCode = rhs.errorCode;
		this->message = rhs.message;
	}
	//destructior
	virtual ~GameError() throw() {};

	//override what from base class
	virtual const char* what() const throw() { return this->getMessage(); }
	const char* getMessage() const throw() { return message.c_str(); } //std::string::c_str Returns a pointer to an array that contains a null-terminated sequence of characters (i.e., a C-string) representing the current value of the string object.
	int getErrorCode() const throw() { return errorCode; }

	//Error codes
	//Negative numbers are fatal errors that may require the game to be shutdown.
	//Positive numbers are warnings that no not require the game to be shutdown.
	static const int FATAL_ERROR = -1;
	static const int WARNING = 1;

private:
	int errorCode;
	std::string message;
};

}

}
#endif // _GRAPHIC_DEFINE_H_2015_11_13