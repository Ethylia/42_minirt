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

vec3 pixelcast(t_context* ctx, const uint x, const uint y)
{
	float top = -tanf(ctx->scene.camera.fov / 2.0f);
	float right = -top * ctx->aspect;

	vec3 posonfocusplane =
		vec3scale((vec3){{{
			right * ((float)(x * 2) / (float)ctx->scene.renderwidth - 1.0f),
			top * ((float)(y * 2) / (float)ctx->scene.renderheight - 1.0f),
			1.0f
		}}}, ctx->scene.camera.focaldist);

	float rad = sqrtf(rand() / (float)RAND_MAX) * ctx->scene.camera.lensradius / ctx->scene.renderwidth;
	float angle = rand() / (float)RAND_MAX * 2.0f * M_PI;

	vec3 posonlens =
		(vec3){{{
			rad * cosf(angle),
			rad * sinf(angle),
			0.0f
		}}};

	vec3 dir = mat3mulvec3(ctx->scene.camera.rot,
		vec3norm(vec3sub(posonfocusplane, posonlens)));

	t_hit hit = raycast((t_ray)
		{
			vec3add(mat3mulvec3(ctx->scene.camera.rot, posonlens), ctx->scene.camera.pos), dir
		}, &ctx->scene);

	if(hit.obj)
		return shade(hit.obj, hit.pos, &ctx->scene, dir);
	else
		return (vec3){{{0.3f, 0.5f, 0.8f}}};
}

void renderregion(t_context *ctx, const t_region region)
{
	for(uint isample = 0; isample < ctx->scene.samples; ++isample)
	{
		for(uint iy = region.y; iy < region.y + region.height; ++iy)
			for(uint ix = region.x; ix < region.x + region.width; ++ix)
				if(ctx->exit)
					return ;
				else
					blend(imgvec(ctx, &region, ix - region.x, iy - region.y),
						pixelcast(ctx, ix, iy));

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
