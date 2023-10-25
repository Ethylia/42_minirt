#include <string.h>
#include <stdlib.h>

#include "scene.h"

#include "util/util.h"

#define INIT_SIZE 8

int scenenew(t_scene* scene)
{
	memset(scene, 0, sizeof(t_scene));
	if(!vecinit(&scene->objvec, sizeof(t_obj), INIT_SIZE))
		return 1;
	if(!vecinit(&scene->matvec, sizeof(t_material), INIT_SIZE))
		return 1;

	t_material defaultmat =
	{
		.metallic = 0.0f,
		.roughness = 1.0f,
		.emit = (vec3){{{0.0f, 0.0f, 0.0f}}},
		.albedo = (vec3){{{0.5f, 0.5f, 0.5f}}}
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
