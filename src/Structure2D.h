#ifndef STRUCTURE_2D_H_2015_11_25
#define STRUCTURE_2D_H_2015_11_25

namespace AA_Engine {

namespace Algorithm {

struct XmFloat3
{
	float x;
	float y;
	float z;
	XmFloat3(float x = 0.0, float y = 0.0, float z = 0.0)
		:x(x), y(y), z(z)
	{
	}
};

union Color32
{
	DWORD color;
	struct
	{
		BYTE red;
		BYTE green;
		BYTE blue;
		BYTE alpha;
	} simpleColor;

	Color32(DWORD color)
		:color(color)
	{
	}

	Color32(BYTE red, BYTE green, BYTE blue, BYTE alpha)
		:simpleColor({red,green,blue,alpha})
	{
	}
};

}

}

#endif // STRUCTURE_2D_H_2015_11_25