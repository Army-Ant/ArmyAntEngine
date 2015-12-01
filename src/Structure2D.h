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
		BYTE alpha;
		BYTE blue;
		BYTE green;
		BYTE red;
	} simpleColor;

	Color32(DWORD color)
		:color(color)
	{
	}

	Color32(BYTE red, BYTE green, BYTE blue, BYTE alpha)
		:simpleColor({alpha,blue,green,red})
	{
	}
};

}

}

#endif // STRUCTURE_2D_H_2015_11_25