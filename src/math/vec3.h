#ifndef VEC3_H
#define VEC3_H

typedef struct
{
	union
	{
		struct
		{
			float	x;
			float	y;
			float	z;
		};
		float	e[3];
	};
} vec3;

vec3 vec3scale(const vec3 v, const float s);
vec3 vec3mult(const vec3 v1, const vec3 v2);
vec3 vec3add(const vec3 v1, const vec3 v2);
vec3 vec3sub(const vec3 v1, const vec3 v2);

float vec3length(const vec3 v);
float vec3dot(const vec3 v1, const vec3 v2);
vec3 vec3norm(const vec3 v);
vec3 vec3reflect(const vec3 v, const vec3 n);
vec3 vec3cross(const vec3 v1, const vec3 v2);

#endif
