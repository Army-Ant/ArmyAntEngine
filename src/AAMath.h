#ifndef _AAMATH_H_17_12_2015_
#define _AAMATH_H_17_12_2015_

#include <cstdlib>

namespace AA_Engine
{
	namespace AA_Algorithm
	{
		//--------------------------------
		//Linear Function
		static int pow(int b, int e)
		{
			if (e == 0)
				return 0;
			if (e == 1)
				return 1;
			if (e % 2 == 0)
				return pow(b*b, e / 2);
			else
				return b*pow(b, e - 1);
		}


		//-----------------------------
		// Random function
		static int random(int low, int high)
		{
			return rand() % (high - low + 1) + low;
		}
		static float random(float low, float high)
		{
			float range = high - low;
			int i_range = int(range * 1000);
			float random_num = rand() % i_range;
			return random_num / 1000 + low;
		}
	}
}
#endif