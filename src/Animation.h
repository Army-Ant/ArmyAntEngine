#ifndef _ANIMATION_H_2015_11_13
#define _ANIMATION_H_2015_11_13

#include "D3d11Use_start.h"
#include <string>

namespace AA_Engine {

namespace AA_Graphic {

struct Animation
{
	std::string name;
	int numFrames;
	int x, y, w, h;
	float fps;
};

}

}
#endif