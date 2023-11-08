#include <stdlib.h>

#include "MLX42/MLX42.h"

#include "render/context.h"
#include "parse/parse.h"
#include "render/render.h"
#include "render/thread.h"

#include "util/util.h"

static void context_resize(int32_t width, int32_t height, void* param)
{
	t_context* const ctx = (t_context*)param;

	if(!ctx->fb || !ctx->fb->instances || !ctx->fb->instances[0].enabled)
		return;
	ctx->fb->instances[0].x = width / 2 - (int32_t)(ctx->fb->width / 2);
	ctx->fb->instances[0].y = height / 2 - (int32_t)(ctx->fb->height / 2);
}

int contextnew(t_context* ctx, int width, int height)
{
	ctx->width = width;
	ctx->height = height;
	ctx->aspect = (float)width / (float)height;
	ctx->mlx = mlx_init(width, height, "miniRT", 1);
	ctx->fb = mlx_new_image(ctx->mlx, width, height);
	if(!ctx->mlx || !ctx->fb)
		return 0;
	intset((int*)ctx->fb->pixels, 0xff000000, width * height);
	mlx_image_to_window(ctx->mlx, ctx->fb, 0, 0);
	mlx_resize_hook(ctx->mlx, &context_resize, ctx);
	return 1;
}

void contextfree(t_context* ctx)
{
	scenefree(&ctx->scene);
	if(ctx->fb)
		mlx_delete_image(ctx->mlx, ctx->fb);
	for(size_t i = 0; i < THREADS; ++i)
	{
		free(ctx->secimg[i]);
		ctx->secimg[i] = 0;
	}
	if(ctx->mlx)
		mlx_terminate(ctx->mlx);
	ctx->fb = 0;
	ctx->mlx = 0;
}
