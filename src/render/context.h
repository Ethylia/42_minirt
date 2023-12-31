#ifndef CONTEXT_H
#define CONTEXT_H

#include <time.h>

#include "thread.h"
#include "obj/scene.h"

typedef struct mlx mlx_t;
typedef struct mlx_image mlx_image_t;

typedef struct s_context
{
	unsigned int width;
	unsigned int height;
	float aspect;
	mlx_t* mlx;
	mlx_image_t* fb;
	t_scene scene;
	pthread_t threads[THREADS];
	int threaddone[THREADS];
	vec3* secimg[THREADS];
	int frameid[THREADS];
	int exit;
	time_t start;
	time_t end;
} t_context;

int contextnew(t_context *ctx, int width, int height);
void contextfree(t_context *ctx);

#endif
