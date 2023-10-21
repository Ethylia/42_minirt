#include <math.h>
#include "mat3.h"

static mat3 rotation(const vec3 axis, const float angle)
{
	const float c = cosf(angle);
	const float s = sinf(angle);
	const float t = 1.0f - c;
	const vec3 axisn = vec3norm(axis);

	return (mat3){{{
			{{{t * axisn.x * axisn.x + c, t * axisn.x * axisn.y - s * axisn.z,
			t * axisn.x * axisn.z + s * axisn.y}}},
			{{{t * axisn.x * axisn.y + s * axisn.z, t * axisn.y * axisn.y + c,
			t * axisn.y * axisn.z - s * axisn.x}}},
			{{{t * axisn.x * axisn.z - s * axisn.y, t * axisn.y * axisn.z + s
			* axisn.x, t * axisn.z * axisn.z + c}}}
		}}};
}

mat3 mat3rotvec(const vec3 v1)
{
	const vec3 up = {{{0.0f, -1.0f, 0.0f}}};
	const vec3 axis = vec3cross(v1, up);
	const float angle = acosf(vec3dot(v1, up) / (vec3length(v1)));

	return rotation(axis, angle);
}

vec3 mat3mulvec3(const mat3 m, const vec3 v)
{
	return (vec3){{{
			m.m00 * v.x + m.m01 * v.y + m.m02 * v.z,
			m.m10 * v.x + m.m11 * v.y + m.m12 * v.z,
			m.m20 * v.x + m.m21 * v.y + m.m22 * v.z
		}}};
}

mat3 mat3inv(const mat3 m)
{
	mat3	m2;

	m2.m00 = m.m11 * m.m22 - m.m12 * m.m21;
	m2.m01 = m.m02 * m.m21 - m.m01 * m.m22;
	m2.m02 = m.m01 * m.m12 - m.m02 * m.m11;
	m2.m10 = m.m12 * m.m20 - m.m10 * m.m22;
	m2.m11 = m.m00 * m.m22 - m.m02 * m.m20;
	m2.m12 = m.m02 * m.m10 - m.m00 * m.m12;
	m2.m20 = m.m10 * m.m21 - m.m11 * m.m20;
	m2.m21 = m.m01 * m.m20 - m.m00 * m.m21;
	m2.m22 = m.m00 * m.m11 - m.m01 * m.m10;
	return (m2);
}

mat3 lookvector(const vec3 axis)
{
	const vec3	up = {{{0.0f, -1.0f, 0.0f}}};
	const vec3	right = vec3norm(vec3cross(up, axis));
	const vec3	newup = vec3norm(vec3cross(axis, right));

	return (mat3){{{
			{{{right.x, newup.x, axis.x}}},
			{{{right.y, newup.y, axis.y}}},
			{{{right.z, newup.z, axis.z}}}
		}}};
}
