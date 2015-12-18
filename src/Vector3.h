#ifndef _VECTOR_H_17_12_2015_
#define _VECTOR_H_17_12_2015_

#include<iostream>

namespace AA_Engine
{
	namespace AA_Algorithm
	{
		struct Vector3
		{
			Vector3();
			//Vector3(const Vector3& copy);
			Vector3(float ax, float ay, float az);

			//unary operator
			//const meanse cannot change the private or protected variable
			Vector3 operator-() const;

			//binary operator
			Vector3 operator+(const Vector3& other)const;
			Vector3 operator-(const Vector3& other)const;
			Vector3 operator=(const Vector3& other);

			//scaler
			Vector3 operator*(float c) const;
			friend Vector3 operator*(float c, const Vector3& v);
			Vector3 operator/(float c)const;

			//utility
			float magnitude();
			float squareMagnitude();

			float x, y, z;
		};
		float distance(const Vector3& a,const Vector3& b);
	}
}

#endif