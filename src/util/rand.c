#include "def.h"

#include <limits.h>
#include <math.h>

uint* randomstate()
{
	static uint state = 0;
	return &state;
}

uint randuint()
{
	uint* s = randomstate();
	*s = *s * 747796405 + 2891336453;
	uint intrand = ((*s >> ((*s >> 28) + 4)) ^ *s) * 277803737;
	return (intrand >> 22) ^ intrand;
}

float normalrand(float stddeviation)
{
	float theta = 2.0f * M_PI * (randuint() / (float)UINT_MAX); 
	float rho = sqrtf(-2.0f * logf(randuint() / (float)UINT_MAX));
	return stddeviation * rho * cosf(theta);
}
