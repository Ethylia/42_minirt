#include "thread.h"

#include "render/context.h"
#include "render/render.h"
#include "util/util.h"
#include "math/clamp.h"

#include <math.h>
#include <string.h>

#include "MLX42/MLX42.h"

typedef struct s_rstate
{
	size_t x;
	size_t y;
	size_t threadindex;
	t_mutex indexmutex;
} t_rstate;

static t_rstate* getstate(void)
{
	static t_rstate state = {
		0,
		0,
		0,
		PTHREAD_MUTEX_INITIALIZER
	};

	return &state;
}

static t_region nextregion(const t_context* ctx, size_t id)
{
	t_rstate* const state = getstate();
	t_region region;

	region.x = state->x;
	region.y = state->y;
	region.width = umin(ctx->scene.regionsize, ctx->width - region.x);
	region.imgid = id;
	state->x += ctx->scene.regionsize;
	if(state->x >= ctx->width)
	{
		state->x = 0;
		state->y += ctx->scene.regionsize;
	}
	region.height = umin(ctx->scene.regionsize, ctx->height - region.y);
	return region;
}

static void renderbox(t_context* ctx, const t_region* reg)
{
	int* const img = (int*)ctx->fb->pixels;

	for(size_t i = 0; i < reg->height; ++i)
	{
		if(i < reg->height / 4 || i > reg->height - reg->height / 4)
		{
			img[(reg->y + i) * ctx->width + reg->x] = 0xFFFFFFFF;
			img[(reg->y + i) * ctx->width + reg->x + reg->width - 1]
				= 0xFFFFFFFF;
		}
	}
	for(size_t i = 0; i < reg->width; ++i)
	{
		if(i < reg->width / 4 || i > reg->width - reg->width / 4)
		{
			img[(reg->y) * ctx->width + reg->x + i] = 0xFFFFFFFF;
			img[(reg->y + reg->height - 1) * ctx->width + reg->x + i]
				= 0xFFFFFFFF;
		}
	}
}

void transferimg(t_context* ctx, const t_region* reg)
{
	const vec3* img = ctx->secimg[reg->imgid];
	const vec3* vec;
	const float id = (float)ctx->frameid[reg->imgid];

	for(size_t i = 0; i < reg->height; ++i)
	{
		for(size_t j = 0; j < reg->width; ++j)
		{
			vec = img + i * reg->width + j;
			float r, g, b;
			r = sqrtf(vec->x / id);
			g = sqrtf(vec->y / id);
			b = sqrtf(vec->z / id);
			const float divider = fmaxf(fmaxf(r, g), b);
			if(divider > 1.0f)
			{
				r /= divider;
				g /= divider;
				b /= divider;

				r += (divider - 1.0f) * (1.0f - r);
				g += (divider - 1.0f) * (1.0f - g);
				b += (divider - 1.0f) * (1.0f - b);

				r = fminf(r, 1.0f);
				g = fminf(g, 1.0f);
				b = fminf(b, 1.0f);
			}
			((int*)ctx->fb->pixels)[(reg->y + i) * ctx->width + reg->x + j]
				= ((int)(r * 255.0f)
					| (int)(g * 255.0f) << 8
					| (int)(b * 255.0f) << 16
					| 0xFF000000);
		}
	}
	if(ctx->frameid[reg->imgid]++ < ctx->scene.samples)
		renderbox(ctx, reg);
}

void* renderthread(void* arg)
{
	t_rstate* const state = getstate();
	t_context* const ctx = arg;
	t_region region;
	int id;

	pthread_mutex_lock(&state->indexmutex);
	id = state->threadindex++;
	pthread_mutex_unlock(&state->indexmutex);
	while(1)
	{
		pthread_mutex_lock(&state->indexmutex);
		if(state->y >= ctx->height)
		{
			ctx->threaddone[id] = 1;
			int alldone = 1;
			for(size_t i = 0; i < THREADS; ++i)
				alldone &= ctx->threaddone[i];
			pthread_mutex_unlock(&state->indexmutex);
			if(alldone)
				ctx->end = time(NULL);
			return NULL;
		}
		region = nextregion(ctx, id);
		pthread_mutex_unlock(&state->indexmutex);
		ctx->frameid[id] = 1;
		memset(ctx->secimg[id], 0, sizeof(vec3) * ctx->scene.regionsize * ctx->scene.regionsize);
		renderregion(ctx, region);
	}
	return NULL;
}
