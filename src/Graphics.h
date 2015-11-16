#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#ifdef _DEBUG
#define D3d_DEBUG_INFO
#endif

#include <d3d11.h>
#include <D3Dx11.h>
#include <xnamath.h>
#include <DxErr.h>
#include "GraphicDefine.h"

namespace AA_Engine {

namespace AA_Graphic {

namespace graphicsNS {
enum DISPLAY_MODE { TOGGLE, FULLSCREEN, WINDOW };
}

//================================================================
//Sprite
//================================================================
struct SpriteData
{
	int width;
	int height;
	float x;
	float y;
	float scale;
	float angle;
	RECT  rect;
	ID3D11Texture2D* texture;
	bool flipHorizontal;
	bool flipVertical;
};

class Graphics
{
protected:
	//variables
	HRESULT   m_hresult;
	HWND	  m_hwnd;
	HINSTANCE m_hInstance;

	D3D_DRIVER_TYPE		    m_driverType;
	D3D_FEATURE_LEVEL	    m_featureLevel;

	ID3D11Device*		    m_pD3dDevice;
	ID3D11DeviceContext*	m_pD3dContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBufferTarget;

public:
	Graphics();
	virtual ~Graphics();
	void ReleaseAll();

	bool Initialize(HINSTANCE hinstance, HWND hw);

	//Display the offscreen backbuffer to the screen
	HRESULT showBackBuffer();

	//test for lost device
	HRESULT getDeviceState();

	//reset the graphics device
	HRESULT reset();

	//
	virtual bool LoadContent();
	virtual void UnloadContent();

};

}

}
#endif