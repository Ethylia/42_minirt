#ifndef RAYCAST_H
#define RAYCAST_H

#include "obj/obj.h"
#include "math/vec3.h"
#include "context.h"

typedef struct s_ray
{
	vec3 pos;
	vec3 dir;
}	t_ray;

typedef struct s_hit
{
	vec3 pos;
	t_obj* obj;
}	t_hit;

float ray_intersect(const t_ray ray, const t_obj* obj);
t_hit raycast(const t_ray ray, const t_scene* s);

#endif
