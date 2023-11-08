#ifndef VEC2_H
#define VEC2_H

typedef struct s_vec2
{
	union
	{
		struct
		{
			float x;
			float y;
		};
		float e[2];
	};
} vec2;

#endif
