#ifndef MAT3_H
#define MAT3_H

#include "math/vec3.h"

typedef struct
{
	union
	{
		float m[9];
		struct
		{
			vec3 x;
			vec3 y;
			vec3 z;
		};
		struct
		{
			float m00;
			float m01;
			float m02;
			float m10;
			float m11;
			float m12;
			float m20;
			float m21;
			float m22;
		};
	};
} mat3;
	
vec3 mat3mulvec3(const mat3 m, const vec3 v);
mat3 lookvector(const vec3 axis);

#endif
