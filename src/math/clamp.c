#include <math.h>
#include "def.h"

float clamp(float x, float min, float max)
{
	if(x < min)
		return min;
	if(x > max)
		return max;
	return x;
}

int imin(int a, int b)
{
	if(a < b)
		return a;
	return b;
}

int imax(int a, int b)
{
	if(a > b)
		return a;
	return b;
}

uint umin(uint a, uint b)
{
	if(a < b)
		return a;
	return b;
}

uint umax(uint a, uint b)
{
	if(a > b)
		return a;
	return b;
}
