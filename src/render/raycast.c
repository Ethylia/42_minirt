#include "raycast.h"
#include "def.h"
#include "math/vec3.h"
#include "math/mat3.h"

#include <math.h>
#include <limits.h>

static float ray_intersect_sphere(const t_ray ray, const t_sphere* obj)
{
	const vec3 oc = { {{ray.pos.x - obj->pos.x,
		ray.pos.y - obj->pos.y, ray.pos.z - obj->pos.z}} };
	const float abc[3] = {
		vec3dot(ray.dir, ray.dir),
		vec3dot(oc, ray.dir),
		vec3dot(oc, oc) - obj->rad * obj->rad
	};
	const float discriminant = abc[1] * abc[1] - abc[0] * abc[2];
	float t[2];

	if(discriminant < 0.0001f)
		return 0.0f;
	t[0] = (-abc[1] - sqrtf(discriminant)) / abc[0];
	t[1] = (-abc[1] + sqrtf(discriminant)) / abc[0];
	if(t[0] > t[1])
		return t[1];
	return t[0];
}

static float ray_intersect_cylinder(t_ray ray, const t_cylinder* obj)
{
	const vec3 oc = { {{ray.pos.x - obj->pos.x,
		ray.pos.y - obj->pos.y, ray.pos.z - obj->pos.z}} };
	const float abc[3] = {
		vec3dot(ray.dir, ray.dir) - vec3dot(ray.dir, obj->axis)
		* vec3dot(ray.dir, obj->axis),
		2 * (vec3dot(oc, ray.dir) - vec3dot(oc, obj->axis)
			* vec3dot(ray.dir, obj->axis)),
		vec3dot(oc, oc) - vec3dot(oc, obj->axis)
		* vec3dot(oc, obj->axis) - obj->rad * obj->rad
	};
	const float discriminant = abc[1] * abc[1] - 4 * abc[0] * abc[2];
	float t[2];

	if(discriminant < 0.0001f)
		return 0.0f;
	t[0] = (-abc[1] - sqrtf(discriminant)) / (2 * abc[0]);
	t[1] = (-abc[1] + sqrtf(discriminant)) / (2 * abc[0]);
	if(t[0] > t[1])
		t[0] = t[1];
	t[1] = vec3dot(vec3sub(vec3add(ray.pos, vec3scale(ray.dir, t[0])),
		obj->pos), obj->axis);
	if(t[1] < 0.0f || t[1] > obj->height)
		return 0.0f;
	return t[0];
}

float ray_intersect_plane(const t_ray ray, const t_plane* obj)
{
	const float denom = vec3dot(obj->ori, ray.dir);
	const float t = vec3dot(vec3sub(obj->pos, ray.pos), obj->ori) / denom;

	if(denom > 0.0001f)
		return 0.0f;
	return t;
}

float ray_intersect(const t_ray ray, const t_obj* obj)
{
	if(obj->type == e_sphere)
		return ray_intersect_sphere(ray, &obj->sphere);
	else if(obj->type == e_plane)
		return ray_intersect_plane(ray, &obj->plane);
	else if(obj->type == e_cylinder)
		return ray_intersect_cylinder(ray, &obj->cylinder);
	return 0.0f;
}

t_hit raycast(const t_ray ray, const t_scene* s)
{
	t_hit hit;
	float t[2];

	hit.obj = NULL;
	t[0] = __FLT_MAX__;
	for(size_t i = 0; i < s->objcount; ++i)
	{
		t[1] = ray_intersect(ray, s->objs + i);
		if(t[1] > 0.0f && t[1] < t[0])
		{
			t[0] = t[1];
			hit.obj = s->objs + i;
		}
	}
	if(hit.obj)
		hit.pos = vec3add(ray.pos, vec3scale(ray.dir, t[0]));
	return hit;
}
