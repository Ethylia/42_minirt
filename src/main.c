#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "parse/parse.h"
#include "render/context.h"
#include "render/raycast.h"
#include "math/vec3.h"
#include "render/render.h"
#include "parse/parse.h"

#include <math.h>

void rtkey(mlx_key_data_t keydata, void* param)
{
	t_context* const ctx = (t_context*)param;

	if(keydata.action == MLX_PRESS)
	{
		if(keydata.key == MLX_KEY_ESCAPE)
		{
			ctx->exit = 1;
			mlx_close_window(ctx->mlx);
		}
	}
}

void loop(void* param)
{
	static int once = 0;
	static int finish = 0;
	t_context* const ctx = (t_context*)param;

	if(ctx->end && !finish)
	{
		printf("Render time: %lds\n", ctx->end - ctx->start);
		finish = 1;
	}
	if(once)
		return;
	render(ctx);
	once = 1;
}

int main(int argc, char const* argv[])
{
	t_context ctx;

	srand(time(NULL));
	if(argc != 2)
		return 1;
	if(scenenew(&ctx.scene))
		return 1;
	if(loadscene(argv[1], &ctx.scene))
	{
		scenefree(&ctx.scene);
		return 1;
	}
	if(!contextnew(&ctx, ctx.scene.renderwidth, ctx.scene.renderheight))
	{
		printf("Error creating context\n");
		return 1;
	}
	mlx_loop_hook(ctx.mlx, loop, &ctx);
	mlx_key_hook(ctx.mlx, rtkey, &ctx);
	mlx_loop(ctx.mlx);
	for(size_t i = 0; i < THREADS; ++i)
		pthread_join(ctx.threads[i], NULL);
	contextfree(&ctx);

	return 0;
}
