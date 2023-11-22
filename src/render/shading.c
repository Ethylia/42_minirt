#include <math.h>
#include <limits.h>

#include "render/shading.h"
#include "math/mat3.h"
#include "math/vec2.h"
#include "obj/obj.h"
#include "util/rand.h"

static vec3	randnorm(const vec3 normal)
{
	vec3 randvec = (vec3){{{normalrand(1.0f), normalrand(1.0f), normalrand(1.0f)}}};
	return vec3norm(vec3add(randvec, normal));
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

static vec3 gettangent(const t_obj* obj, const vec3 normal)
{
	vec3 up;

	if(obj->type == e_sphere)
	{
		up = (vec3){{{0.0f, -1.0f, 0.0f}}};
		if(fabsf(normal.y) > 0.99f)
			up = (vec3){{{1.0f, 0.0f, 0.0f}}};
		return vec3norm(vec3cross(normal, up));
	}
	else if(obj->type == e_plane)
	{
		up = (vec3){{{0.0f, -1.0f, 0.0f}}};
		if(fabsf(normal.y) > 0.99f)
			up = (vec3){{{0.0f, 0.0f, -1.0f}}};
		return vec3norm(vec3cross(normal, up));
	}
	else
		return vec3norm(vec3cross(normal, obj->cylinder.axis));
}

static vec3 normalmap(const t_obj* obj, const vec3 normal, const vec2 uv, const texture* map)
{
	const vec3 tangent = gettangent(obj, normal);
	const vec3 bitangent = vec3norm(vec3cross(normal, tangent));
	const mat3 tbn = (mat3){{{
		tangent.x, bitangent.x, normal.x,
		tangent.y, bitangent.y, normal.y,
		tangent.z, bitangent.z, normal.z
	}}};
	const vec3 n = textureuv(map, uv);
	return vec3norm(vec3add(vec3scale(vec3norm(mat3mulvec3(tbn, n)), 0.5f), normal));
}

static vec2 getuv(const t_obj* obj, const vec3 normal, const vec3 hit)
{
	vec2 uv;
	if(obj->type == e_sphere)
	{
		uv.y = (atan2f(normal.z, normal.x)) * (obj->sphere.rad);
		uv.x = (asinf(normal.y)) * (obj->sphere.rad);
	}
	else if(obj->type == e_plane)
	{
		const vec3 up = vec3norm((obj->plane.ori.x == 0.0f && obj->plane.ori.y == 0.0f) ?
			(vec3){{{
				0.0f,
				1.0f,
				0.0f
			}}} :
			(vec3){{{
				0.0f,
				0.0f,
				1.0f
			}}});
		const vec3 v = vec3cross(normal, up);
		uv.x = vec3dot(up, hit);
		uv.y = vec3dot(v, hit);
	}
	else // cylinder
	{
		uv.x = vec3dot(vec3sub(hit, obj->cylinder.pos), obj->cylinder.axis) / (obj->cylinder.rad) * 0.25f;
		uv.y = atan2f(normal.z, normal.x) * 0.225f;
	}
	return uv;
}

static vec3 vec3clerp(const vec3 v1, const vec3 v2, const float t)
{
	const float theta = acosf(vec3dot(v1, v2));
	const float sintheta = sinf(theta);
	const float sin1t = sinf((1.0f - t) * theta);
	const float sint = sinf(t * theta);
	return vec3norm(vec3add(vec3scale(v1, sin1t / sintheta),
		vec3scale(v2, sint / sintheta)));
}

vec3 shade(t_ray ray, const t_scene* s)
{
	vec3 light = {{{0.0f, 0.0f, 0.0f}}};
	vec3 color = {{{1.0f, 1.0f, 1.0f}}};
	vec2 uv;
	for(size_t i = 0; i < s->maxdepth; ++i)
	{
		const t_hit h = raycast(ray, s);
		if(!h.obj)
		{
			light = vec3add(light, vec3mult(s->a_light.color, color));
			break;
		}
		const t_material* mat = h.obj->mat;
		vec3 norm = getnormal(h.obj, h.pos);
		if(mat->type == mat_checker)
		{
			uv = getuv(h.obj, norm, h.pos);
			const int checkx = (fabsf(fmodf(floorf(uv.x * 10.0f), 2.0f)) >= 1.0f);
			const int checky = (fabsf(fmodf(floorf(uv.y * 10.0f), 2.0f)) >= 1.0f);
			if(checkx ^ checky)
				mat = s->mats + mat->checkermat;
		}
		vec3 c = mat->albedo;
		float r = 1.0f - mat->roughness;
		if(mat->type == mat_image)
		{
			uv = getuv(h.obj, norm, h.pos);
			if(mat->talbedo)
				c = textureuv(mat->talbedo, uv);
			if(mat->troughness)
				r = 1.0f - textureuvf(mat->troughness, uv);
		}
		if(mat->tnormal)
			norm = normalmap(h.obj, norm, uv, mat->tnormal);
		const vec3 specular = vec3reflect(ray.dir, norm);
		const vec3 diffuse = randnorm(norm);
		const float specchance = (r) * (1.0f - mat->metallic);
		const float fresnel = 1.0f - sqrtf(-vec3dot(ray.dir, norm)) * 0.95f;

		light = vec3add(light, vec3mult(vec3scale(mat->emit, 3.0f), color));

		if(randuint() / (float)UINT_MAX < (specchance * fresnel))
		{
			color = vec3scale(color, 0.95f);
			ray = (t_ray){h.pos, specular};
		}
		else
		{
			color = vec3mult(color, c);
			ray = (t_ray){h.pos, vec3clerp(diffuse, specular, mat->metallic * (r))};
		}
		if(color.x + color.y + color.z < 0.001f)
			break; // early out if color is too dark to see
		ray.pos = vec3add(h.pos, vec3scale(norm, 0.0001f));
	}
	return light;
}
