#include <math.h>
#include "mat3.h"

vec3 mat3mulvec3(const mat3 m, const vec3 v)
{
	return (vec3){{{
			m.m00 * v.x + m.m01 * v.y + m.m02 * v.z,
			m.m10 * v.x + m.m11 * v.y + m.m12 * v.z,
			m.m20 * v.x + m.m21 * v.y + m.m22 * v.z
		}}};
}

mat3 lookvector(const vec3 axis)
{
	const vec3	up = {{{0.0f, -1.0f, 0.0f}}};
	const vec3	right = vec3norm(vec3cross(up, axis));
	const vec3	newup = vec3norm(vec3cross(axis, right));

	return (mat3){{{
			right.x, newup.x, axis.x,
			right.y, newup.y, axis.y,
			right.z, newup.z, axis.z
		}}};
}
