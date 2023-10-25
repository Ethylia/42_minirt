#ifndef OBJ_H
#define OBJ_H

#include "def.h"
#include "math/mat3.h"
#include "material.h"

typedef struct s_a_light
{
	vec3 color;
} t_a_light;

typedef struct s_camera
{
	vec3 pos;
	vec3 ori;
	mat3 rot;
	float fov;
	float lensradius;
	float focaldist;
} t_camera;

typedef struct s_sphere
{
	vec3 pos;
	float rad;
} t_sphere;

typedef struct s_plane
{
	vec3 pos;
	vec3 ori;
} t_plane;

typedef struct s_cylinder
{
	vec3 pos;
	vec3 axis;
	float rad;
	float height;
} t_cylinder;

typedef enum e_type
{
	e_a_light,
	e_camera,
	e_light,
	e_sphere,
	e_plane,
	e_cylinder,
} t_type;

typedef struct s_obj
{
	t_type type;
	t_material* mat;
	union
	{
		t_cylinder cylinder;
		t_plane plane;
		t_sphere sphere;
		vec3 pos;
	};
} t_obj;

#endif
