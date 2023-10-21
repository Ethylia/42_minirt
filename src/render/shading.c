#include <math.h>
#include <stdlib.h>

#include "render/shading.h"
#include "math/math.h"
#include "math/mat3.h"
#include "math/vec3.h"
#include "obj/obj.h"

static vec3	randnorm(const vec3 raydir, const vec3 normal)
{
	float yaw = (rand() / ((float)RAND_MAX / (M_PI * 2)));
	float pitch = (rand() / ((float)RAND_MAX / (M_PI * 2)));
	vec3 randvec = vec3norm((vec3){{{
		cosf(yaw) * sinf(pitch),
		sinf(yaw) * sinf(pitch),
		cosf(pitch),
	}}});
	if(vec3dot(randvec, normal) < 0.0f)
		randvec = vec3scale(randvec, -1.0f);
	return vec3reflect(raydir, randvec);
}

static vec3	getnormal(const t_obj* obj, const vec3 hit)
{
	vec3 proj;

	if(obj->type == e_sphere)
		return vec3norm(vec3sub(hit, obj->sphere.pos));
	else if(obj->type == e_plane)
		return obj->plane.ori;
	else
	{
		proj = vec3sub(hit, obj->cylinder.pos);
		return vec3norm(vec3sub(proj, vec3scale(obj->cylinder.axis,
			vec3dot(proj, obj->cylinder.axis))));
	}
}

vec3 shade(t_obj* obj, vec3 hit, const t_scene* s, vec3 ray)
{
	t_hit h = {hit, obj};

	vec3 color = {{{1.0f, 1.0f, 1.0f}}};
	vec3 l = obj->emit;
	for(size_t i = 1; i < s->maxdepth; ++i)
	{
		color = vec3mult(color, h.obj->color);
		const vec3 norm = getnormal(h.obj, h.pos);
		ray = randnorm(ray, norm);
		hit = vec3add(h.pos, vec3scale(norm, 0.0001f));
		h = raycast((t_ray){hit, ray}, s);
		if(!h.obj)
		{
			// sky light
			l = vec3add(l, vec3mult(s->a_light.color, color));
			break;
		}
		l = vec3add(l, vec3mult(h.obj->emit, color));
	}

	return l;
}
