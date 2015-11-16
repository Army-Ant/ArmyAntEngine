#ifndef _BASEGAMEMANAGER_H_
#define _BASEGAMEMANAGER_H_

#include<Windows.h>
#include<mmsystem.h>
#include"gameError.h"
#include"Graphics.h"
#include"Constants.h"

class BaseGameManager
{
protected:
	HWND hwnd; // for window handle
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timerFreq;
	float frameTime;
	float fps;
	DWORD sleepTime;
	bool  paused;
	bool initialized;
	//====================
	Graphics* m_graphics;

public:
	BaseGameManager();
	virtual ~BaseGameManager();
//==========================================================================================
// basic function
//==========================================================================================
	//window message handler
	LRESULT messageHandler(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam);

	//initalize the game
	virtual void Initialize(HINSTANCE hInstance,HWND hwnd);

	//call run repeatedly by the main message loop in WinMain
	virtual void Run(HWND hw);

	//call when the graphics device is lost
	virtual void ReleaseAll();

	//recreate all surfaces 
	//delete all reserved memory 
	virtual void DeleteAll();

	//exit the game
	void ExitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }
//==========================================================================================
// Graphics stuff
//==========================================================================================
	//render game items
	virtual void RenderGame();

	//handle lost graphics device
	virtual void HandleLostGraphicsDevice();

	

		
};
#endif