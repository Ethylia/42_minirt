#ifndef SCENE_H
#define SCENE_H

#include "obj.h"

typedef struct s_scene
{
	t_light* lights;
	size_t lightc;
	size_t lightsize;
	t_obj* objs;
	size_t objc;
	size_t objsize;
	t_a_light a_light;
	t_camera camera;
	uint samples;
	uint maxdepth;
	uint regionsize;
	uint renderwidth;
	uint renderheight;
}	t_scene;

int scenenew(t_scene* scene);
void scenefree(t_scene* scene);
t_light* sceneaddlight(t_scene* scene);
t_obj* sceneaddobj(t_scene* scene);

#endif
