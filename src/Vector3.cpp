#include "Vector3.h"

AA_Engine::AA_Algorithm::
Vector3::Vector3()
{

}

AA_Engine::AA_Algorithm::Vector3::Vector3(float ax,float ay,float az)
{

}

AA_Engine::AA_Algorithm::Vector3
AA_Engine::AA_Algorithm::Vector3::operator-()const
{
	Vector3 result;
	result.x = -x;
	result.y = -y;
	result.z = -z;
	return result;
}

AA_Engine::AA_Algorithm::Vector3
AA_Engine::AA_Algorithm::Vector3::operator+(const Vector3& other) const
{
	Vector3 result;
	result.x = x + other.x;
	result.y = y + other.y;
	result.z = z + other.z;
	return result;
}
AA_Engine::AA_Algorithm::Vector3
AA_Engine::AA_Algorithm::Vector3::operator-(const Vector3& other)const
{
	Vector3 result;
	result.x = x + other.x;
	result.y = y + other.y;
	result.z = z + other.z;
	return result;
}
AA_Engine::AA_Algorithm::Vector3
AA_Engine::AA_Algorithm::Vector3::operator=(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}
AA_Engine::AA_Algorithm::Vector3
AA_Engine::AA_Algorithm::Vector3::operator*(float c)const
{
	Vector3 result;
	result.x = x*c;
	result.y = y*c;
	result.z = z*c;
	return result;
}
AA_Engine::AA_Algorithm::Vector3
AA_Engine::AA_Algorithm::Vector3::operator*(float c,const Vector3& v)
{
	return v*c;
}
AA_Engine::AA_Algorithm::Vector3
AA_Engine::AA_Algorithm::Vector3::operator/(float c) const
{
	Vector3 result;
	result.x = x / c;
	result.y = y / c;
	result.z = z / c;
	return result;
}

float AA_Engine::AA_Algorithm::Vector3::squareMagnitude()
{
	return x*x + y*y + z*z;
}
float AA_Engine::AA_Algorithm::Vector3::magnitude()
{
	return sqrt(squareMagnitude());
}
float AA_Engine::AA_Algorithm::distance(const Vector3& a, const Vector3& b)
{
	return (a - b).magnitude();
}

