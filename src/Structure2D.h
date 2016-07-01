#ifndef STRUCTURE_2D_H_2015_11_25
#define STRUCTURE_2D_H_2015_11_25

namespace AA_Engine {

namespace Algorithm {

struct XmFloat2
{
public:
	float x;
	float y;
	XmFloat2(float x = 0.0, float y = 0.0)
		:x(x), y(y)
	{
	}
	void Set(float posx, float posy) {
		x = posx;
		y = posy;
	}
	uint32 ToDword()const 
	{
		auto ret = *reinterpret_cast<const uint32*>(this);
		return ret;
	}
};

struct XmFloat3 : public XmFloat2
{
	float z;
	XmFloat3(float x = 0.0, float y = 0.0, float z = 0.0)
		:XmFloat2(x, y), z(z)
	{
	}
	void Set(float posx, float posy, float posz)
	{
		XmFloat2::Set(posx, posy);
		z = posz;
	}
};

struct XmFloat4 : public XmFloat3
{
	float w;
	XmFloat4(float x = 0.0, float y = 0.0, float z = 0.0, float w = 0.0)
		:XmFloat3(x, y, z), w(w)
	{
	}
	void Set(float posx, float posy, float posz, float posw)
	{
		XmFloat3::Set(posx, posy, posz);
		w = posw;
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