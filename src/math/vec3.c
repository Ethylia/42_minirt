#include <math.h>

#include "vec3.h"

vec3	vec3scale(const vec3 v, const float s)
{
	vec3 vs;

	vs.x = v.x * s;
	vs.y = v.y * s;
	vs.z = v.z * s;
	return vs;
}

vec3 vec3mult(const vec3 v1, const vec3 v2)
{
	vec3 v;

	v.x = v1.x * v2.x;
	v.y = v1.y * v2.y;
	v.z = v1.z * v2.z;
	return v;
}

vec3 vec3add(const vec3 v1, const vec3 v2)
{
	vec3 v;

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	return v;
}

int vec3color(const vec3 v)
{
	return (int)(fmaxf(fminf(v.x, 1.0f), 0.0f) * 255.0f) << 24
		| (int)(fmaxf(fminf(v.y, 1.0f), 0.0f) * 255.0f) << 16
		| (int)(fmaxf(fminf(v.z, 1.0f), 0.0f) * 255.0f) << 8 | 0xff;
}

vec3 vec3sub(const vec3 v1, const vec3 v2)
{
	vec3 v;

	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	return v;
}

float vec3length(const vec3 v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 vec3norm(const vec3 v)
{
	float len;
	vec3 n;

	len = vec3length(v);
	n.x = v.x / len;
	n.y = v.y / len;
	n.z = v.z / len;
	return n;
}

float vec3dot(const vec3 v1, const vec3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 vec3reflect(const vec3 v, const vec3 n)
{
	return vec3sub(v, vec3scale(n, 2.0 * vec3dot(n, v)));
}

vec3 vec3cross(const vec3 v1, const vec3 v2)
{
	vec3 v;

	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}
