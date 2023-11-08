#ifndef SCENE_H
#define SCENE_H

#include "obj.h"
#include "obj/material.h"
#include "obj/texture.h"
#include "util/vector.h"
#include "def.h"

typedef struct s_scene
{
	union
	{
		struct
		{
			t_obj* objs;
			size_t objcount;
		};
		vector objvec;
	};
	union
	{
		struct
		{
			t_material* mats;
			size_t matcount;
		};
		vector matvec;
	};
	union
	{
		struct
		{
			texture* texs;
			size_t texcount;
		};
		vector texvec;
	};

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
t_obj* sceneaddobj(t_scene* scene);
t_material* sceneaddmat(t_scene* scene);
texture* sceneaddtex(t_scene* scene);

#endif
