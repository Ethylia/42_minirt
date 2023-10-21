#include "render/render.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "math/vec3.h"
#include "math/mat3.h"
#include "render/raycast.h"
#include "render/shading.h"
#include "thread.h"

#include <math.h>

static void blend(vec3* c1, const vec3 c2)
{
	*c1 = vec3add(*c1, c2);
}

static vec3* imgvec(t_context* ctx, const t_region* reg,
	const unsigned int x, const unsigned int y)
{
	return (&(ctx->secimg[reg->imgid][y * reg->width + x]));
}

vec3 pixelcast(t_context* ctx, const t_ray ray,
	const unsigned int x, const unsigned int y)
{
	vec3 color;
	t_hit hit;

	hit = raycast((t_ray){vec3norm((vec3){{{
			ray.pos.x + ((rand() / (float)(RAND_MAX)) - 0.5f) * 0.0045f,
			ray.pos.y + ((rand() / (float)(RAND_MAX)) - 0.5f) * 0.0045f,
			ray.pos.z + ((rand() / (float)(RAND_MAX)) - 0.5f) * 0.0045f,
		}}}), ray.dir}, &ctx->scene);
	if(hit.obj)
		color = shade(hit.obj, hit.pos, &ctx->scene, ray.dir);
	else
		color = (vec3){{{0.3f, 0.5f, 0.8f}}};
	return color;
}

void renderregion(t_context *ctx, const t_region region)
{
	const float fov = tanf(ctx->scene.camera.fov / 2.0f);

	for(uint isample = 0; isample < ctx->scene.samples; ++isample)
	{
		for(uint iy = region.y; iy < region.y + region.height; ++iy)
		{
			for(uint ix = region.x; ix < region.x + region.width; ++ix)
			{
				if(ctx->exit)
					return ;
				const t_ray ray = {ctx->scene.camera.pos,
					mat3mulvec3(ctx->scene.camera.rot, (vec3){{{
							(2.0f * ((ix + 0.5f) / ctx->width) - 1.0f) * ctx->aspect * fov,
							-(2.0f * ((iy + 0.5f) / ctx->height) - 1.0f) * fov,
							1.0f
						}}})
					};
				blend(imgvec(ctx, &region, ix - region.x, iy - region.y),
					pixelcast(ctx, ray, ix, iy));
			}
		}
		transferimg(ctx, &region);
	}
}

void	render(t_context *ctx)
{
	ctx->start = time(NULL);
	for(size_t i = 0; i < THREADS; ++i)
	{
		ctx->secimg[i] = malloc(ctx->scene.regionsize * ctx->scene.regionsize * sizeof(vec3));
		pthread_create(&ctx->threads[i], NULL, renderthread, (void*)ctx);
	}
}
