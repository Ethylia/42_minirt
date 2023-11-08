#include "scene.h"

#include <stdlib.h>

#include "util/util.h"

#define INIT_SIZE 8

int scenenew(t_scene* scene)
{
	if(!vecinit(&scene->objvec, sizeof(t_obj), INIT_SIZE))
		return 1;
	if(!vecinit(&scene->matvec, sizeof(t_material), INIT_SIZE))
		return 1;
	if(!vecinit(&scene->texvec, sizeof(texture), INIT_SIZE))
		return 1;

	texture* defaulttex = sceneaddtex(scene);
	if(!defaulttex)
		return 1;
	defaulttex->data = malloc(4 * sizeof(vec3));
	if(!defaulttex->data)
		return 1;
	defaulttex->width = 2;
	defaulttex->height = 2;
	((vec3*)defaulttex->data)[0] = (vec3){{{0.01f, 0.01f, 0.01f}}};
	((vec3*)defaulttex->data)[1] = (vec3){{{0.75f, 0.06f, 0.75f}}};
	((vec3*)defaulttex->data)[2] = (vec3){{{0.75f, 0.06f, 0.75f}}};
	((vec3*)defaulttex->data)[3] = (vec3){{{0.01f, 0.01f, 0.01f}}};

	t_material defaultmat =
	{
		.type = mat_image,
		.metallic = 0.0f,
		.roughness = 1.0f,
		.emit = (vec3){{{0.0f, 0.0f, 0.0f}}},
		.albedo = (vec3){{{0.5f, 0.5f, 0.5f}}},
		.talbedo = defaulttex
	};

	vecpush(&scene->matvec, &defaultmat);

	scene->a_light.color = (vec3){{{0.0f, 0.0f, 0.0f}}};

	scene->maxdepth = 5;
	scene->regionsize = 32;

	scene->camera.pos = (vec3){{{0.0f, 0.0f, 0.0f}}};
	scene->camera.ori = (vec3){{{0.0f, 0.0f, 1.0f}}};
	scene->camera.fov = 1.5f;
	scene->camera.lensradius = 0.0f;
	scene->camera.focaldist = 1.0f;

	return 0;
}

void scenefree(t_scene* scene)
{
	vecfree(&scene->objvec);
	vecfree(&scene->matvec);
	for(size_t i = 0; i < scene->texvec.size; ++i)
		freetexture(&scene->texs[i]);
	vecfree(&scene->texvec);
}

t_obj* sceneaddobj(t_scene* scene)
{
	if(vecpush(&scene->objvec, &(t_obj){0}))
		return &(scene->objs[scene->objvec.size - 1]);
	else
		return 0;
}

t_material* sceneaddmat(t_scene* scene)
{
	if(vecpush(&scene->matvec, &(t_material){0}))
		return &(scene->mats[scene->matvec.size - 1]);
	else
		return 0;
}

texture* sceneaddtex(t_scene* scene)
{
	if(vecpush(&scene->texvec, &(texture){0}))
		return &(scene->texs[scene->texvec.size - 1]);
	else
		return 0;
}
