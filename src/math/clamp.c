#include <math.h>
#include "def.h"

int imin(int a, int b)
{
	if(a < b)
		return a;
	return b;
}

uint umin(uint a, uint b)
{
	if(a < b)
		return a;
	return b;
}
