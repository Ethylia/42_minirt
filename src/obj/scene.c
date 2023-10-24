#include <string.h>
#include <stdlib.h>

#include "scene.h"

#include "util/util.h"

#define INIT_SIZE 8

int scenenew(t_scene* scene)
{
	memset(scene, 0, sizeof(t_scene));
	if(!(scene->objs = memalloc(sizeof(t_obj) * INIT_SIZE, 0, 0)))
		return 1;
	scene->objsize = INIT_SIZE;
	if(!(scene->lights = memalloc(sizeof(t_light) * INIT_SIZE, 0, 0)))
		return 1;
	scene->lightsize = INIT_SIZE;

	scene->maxdepth = 5;
	scene->regionsize = 32;

	scene->camera.pos = (vec3){{{0.0f, 0.0f, 0.0f}}};
	scene->camera.ori = (vec3){{{0.0f, 0.0f, 1.0f}}};
	scene->camera.fov = 1.5f;
	scene->camera.lensradius = 0.0005f;
	scene->camera.focaldist = 0.05f;

	return 0;
}

void scenefree(t_scene* scene)
{
	free(scene->objs);
	scene->objs = 0;
	free(scene->lights);
	scene->lights = 0;
}

t_light* sceneaddlight(t_scene* scene)
{
	if(scene->lightc >= scene->lightsize)
	{
		if(!(scene->lights = memalloc(sizeof(t_light) * scene->lightsize * 2,
			sizeof(t_light) * scene->lightsize, scene->lights)))
			return 0;
		scene->lightsize *= 2;
	}
	return scene->lights + scene->lightc++;
}

t_obj* sceneaddobj(t_scene* scene)
{
	if(scene->objc >= scene->objsize)
	{
		if(!(scene->objs = memalloc(sizeof(t_obj) * scene->objsize * 2,
			sizeof(t_obj) * scene->objsize, scene->objs)))
			return 0;
		scene->objsize *= 2;
	}
	return &(scene->objs[scene->objc++]);
}
