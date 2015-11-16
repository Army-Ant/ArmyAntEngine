#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <Windows.h>

//================================================================
//Useful macros
//================================================================
//Safely delete pointer referenced item
#define SAFF_DELETE(ptr) {if(ptr){delete (ptr); (ptr)=NULL;}}
//Safely release pointer referenced item
#define SAFF_RELEASE(ptr) {if(ptr){ptr->Release();(ptr)=NULL;}}
//Safely delete pointer referenced array
#define SAFF_DELETE_ARRAY(ptr) {if(ptr){delete[](ptr);(ptr)=NULL;}}

#define SAFF_ON_LOST_DEVICE(ptr){if(ptr){ptr->onLostDevice();}}

#define SAFF_ON_RESET_DEVICE(ptr){if(ptr){ptr->onResetDevice();}}

#define TRANSCOLOR SETCOLOR_ARGB(0,255,0,255) //transparency color (magenta)

//================================================================================
//window
//================================================================================
const char CLASS_NAME[] = "ArmyAnt_what_ever";
const char GAME_TITLE[] = "ArmyAnt_GameI";
const bool FULLSCREEN = false;
const UINT GAME_WIDTH = 1024;
const UINT GAME_HEIGHT = 650;
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
const UCHAR ESC_KEY = VK_ESCAPE;
const UCHAR WARRIOR_LEFT_KEY = VK_LEFT;
const UCHAR WARRIOR_UP_KEY = VK_UP;
const UCHAR WARRIOR_DOWN_KEY = VK_DOWN;
const UCHAR WARRIOR_RIGHT_KEY = VK_RIGHT;
//================================================================================
//graphic images
//================================================================================


//================================================================================
//
//================================================================================

#endif // ! _CONSTANTS_H_
