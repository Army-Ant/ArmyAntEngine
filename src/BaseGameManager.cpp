#include "BaseGameManager.h"

BaseGameManager::BaseGameManager()
{
	paused = false;
	initialized = false;
}
BaseGameManager::~BaseGameManager()
{
	DeleteAll();
	ShowCursor(true);
}
LRESULT BaseGameManager::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (initialized)
	{
		switch (msg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			case WM_KEYDOWN: case WM_SYSKEYDOWN://key down
				//input->keyDown(wParam);
				return 0;
			case WM_KEYUP: case WM_SYSKEYUP://key up
				//input->keyIn(wParam);
				return 0;
			case WM_CHAR: //character entered
				//input->keyIn(wParam);
			case WM_MOUSEMOVE: //mouse moved
				//input->mouseIn(lParam);
				return 0;
			case WM_INPUT: //raw mouse data in
				//input->mouseRawIn(lParam)
				return 0;
			case WM_LBUTTONDOWN: //letf mouse botton down
				//input->setMouseLButton(true);
				//input->mouseIn(lParam);
				return 0;
			case WM_LBUTTONUP:
				//input->setMouseButton(false);
				//input->mouseIn(lParam);
				return 0;
			case WM_MBUTTONDOWN: //middle mouse botton down
				//input->setMouseLButton(true);
				//input->mouseIn(lParam);
				return 0;
			case WM_MBUTTONUP:
				//input->setMouseButton(false);
				//input->mouseIn(lParam);
				return 0;
			case WM_RBUTTONDOWN: //right mouse botton down
				//input->setMouseLButton(true);
				//input->mouseIn(lParam);
				return 0;
			case WM_RBUTTONUP:
				//input->setMouseButton(false);
				//input->mouseIn(lParam);
				return 0;
			case WM_DEVICECHANGE://check for controllers
				//input->checkControllers();
				return 0;
		}
	}
	return DefWindowProc(hwnd,msg,wParam,lParam); // let Windows OS handle it
}
void BaseGameManager::Initialize(HINSTANCE hInstance,HWND hw)
{
	hwnd = hw; //save windows handle

	//initialze graphics
	 m_graphics = new Graphics();
	 m_graphics->Initialize(hInstance,hw);

	//initialize input, do not capture mouse
	//input->initialze(hwnd,false);

	//set up high resolution timer
	if (QueryPerformanceFrequency(&timerFreq) == false)//the function stores the frequency of the high-performance timer in the variable timerFreq
		throw(GameError(gameErrorNS::FATAL_ERROR,"Error initialzizing high res timer"));
	QueryPerformanceCounter(&timeStart);//get start time
	//initializetion completed!
	initialized = true;
}
void BaseGameManager::Run(HWND hw)
{
	if (m_graphics == NULL)
		return;
	
	//caculate elapsed time of last frame, save in framTime
	QueryPerformanceCounter(&timeEnd);

	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart)
				/(float)timerFreq.QuadPart;
	
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		return;
	}

	if (frameTime > 0.0)
	{
		fps = (fps*0.99f) + (0.01f / frameTime);
	}

	timeStart = timeEnd;
//=========================================================================
//Run logic
//=======================================================================
	if (!paused)
	{
		//Update();
		//Ai()
		//Collisions();
	}
	RenderGame();
}
void BaseGameManager::RenderGame()
{
	// Graphics render staff
}
void BaseGameManager::HandleLostGraphicsDevice()
{

}
void BaseGameManager::DeleteAll()
{
	ReleaseAll();
	//SAFE_DELETE(graohics);
	//SAFE_DELETE(input);
}
void BaseGameManager::ReleaseAll()
{
	SAFF_DELETE(m_graphics);
}