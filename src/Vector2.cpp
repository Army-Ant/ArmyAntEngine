#include "Vector2.h"


AA_Engine::Algorithm::Vector2::Vector2()
{

}

AA_Engine::Algorithm::Vector2::Vector2(float ax, float ay)
{
	x = ax;
	y = ay;
}
//===========================================
AA_Engine::Algorithm::Vector2
AA_Engine::Algorithm::Vector2::operator-() const
{
	Vector2 result;
	result.x = -x;
	result.y = -y;
	return result;
}

AA_Engine::Algorithm::Vector2 
AA_Engine::Algorithm::Vector2::operator+(const Vector2& other) const
{
	Vector2 result;
	result.x = x + other.x;
	result.y = y + other.y;
	return result;
}

AA_Engine::Algorithm::Vector2 
AA_Engine::Algorithm::Vector2::operator-(const Vector2& other) const
{
	Vector2 result;
	result.x = x - other.x;
	result.y = y - other.y;
	return result;
}

AA_Engine::Algorithm::Vector2
AA_Engine::Algorithm::Vector2::operator=(const Vector2& copy)
{
	x = copy.x;
	y = copy.y;
	return copy;
}

AA_Engine::Algorithm::Vector2
AA_Engine::Algorithm::Vector2::operator*(float c) const
{
	Vector2 result;
	result.x = x*c;
	result.y = y*c;
	return result;
}

AA_Engine::Algorithm::Vector2
AA_Engine::Algorithm::operator*(float c, const Vector2& v)
{
	return v * c;
}

AA_Engine::Algorithm::Vector2 
AA_Engine::Algorithm::Vector2::operator/(float c) const
{
	Vector2 result;
	result.x = x / c;
	result.y = y / c;
	return result;
}


float AA_Engine::Algorithm::Vector2::squareMagnitude()
{
	return x*x + y*y;
}

float AA_Engine::Algorithm::Vector2::magnitude()
{
	return sqrt(squareMagnitude());
}

float AA_Engine::Algorithm::distance(const Vector2& a, const Vector2& b)
{
	return (a - b).magnitude();
}
