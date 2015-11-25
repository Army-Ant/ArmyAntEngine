#ifndef TEMPLATES_2D_HPP_2015_11_25
#define TEMPLATES_2D_HPP_2015_11_25

namespace AA_Engine {

namespace Algorithm {

template <class T_DATA>
class Vector2D
{
public:
	Vector2D(T_DATA x = 0, T_DATA y = 0);

public:
	T_DATA x;
	T_DATA y;
};

template <class T_DATA>
class Vector3D : public Vector2D<T_DATA>
{
public:
	Vector3D(T_DATA x = 0, T_DATA y = 0, T_DATA z = 0);

public:
	T_DATA z;
};

typedef Vector2D<float> Point2D;
typedef Vector3D<float> Point3D;

template <class T_DATA>
class Size2D
{
public:
	Size2D(T_DATA width = 0, T_DATA height = 0);

public:
	T_DATA width;
	T_DATA height;
};

template <class T_DATA>
class Size3D :public Size2D<T_DATA>
{
public:
	Size3D(T_DATA width = 0, T_DATA height = 0, T_DATA depth = 0);

public:
	T_DATA depth;
};

template <class T_DATA>
class Rect2D :public Vector2D<T_DATA>, public Size2D<T_DATA>
{
public:
	Rect2D(T_DATA x = 0, T_DATA y = 0, T_DATA width = 0, T_DATA height = 0);
};

template<class T_DATA>
class Rect3D :public Vector3D<T_DATA>, public Size3D<T_DATA>
{
public:
	Rect3D(T_DATA x = 0, T_DATA y = 0, T_DATA z = 0, T_DATA width = 0, T_DATA height = 0, T_DATA depth = 0);
};

}

}

#endif // TEMPLATES_2D_HPP_2015_11_25