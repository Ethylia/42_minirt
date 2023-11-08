#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "util/util.h"
#include "math/mat3.h"
#include "obj/scene.h"
#include "parseobj.h"

#define MAX_MATERIALS 64

int	parseambient(const char* line, t_scene* scene)
{
	float ratio;
	int r = 0;
	if((r = parsedouble(&line, &ratio)) ||
		(r = parsecolor(&line, scene->a_light.color.e)))
		return r;

	if(ratio < 0.0f)
		return 2;

	scene->a_light.color = vec3scale(scene->a_light.color, ratio);

	line += countws(line);
	if(*line != '\n' && *(line) != 0)
		return 3;

	return 0;
}

int	parsesettings(const char* line, t_scene* scene)
{
	if(parseuint(&line, &scene->renderwidth) ||
		parseuint(&line, &scene->renderheight) ||
		parseuint(&line, &scene->samples))
		return 3;
	if(scene->samples < 1 || scene->renderwidth < 1 || scene->renderheight < 1)
		return 2;
	line += countws(line);
	if(*line != '\n' && *(line) != 0)
	{
		if(parseuint(&line, &scene->maxdepth))
			return 3;
		if(scene->maxdepth < 1)
			return 2;
		line += countws(line);
		if(*line != '\n' || *(line) != 0)
		{
			if(parseuint(&line, &scene->regionsize))
				return 3;
			if(scene->regionsize < 1)
				return 2;
		}
	}

	line += countws(line);
	if(*line != '\n' && *(line) != 0)
		return 3;

	return 0;
}

int	parsecamera(const char* line, t_scene* scene)
{
	int r = 0;
	if((r = parsevec(&line, scene->camera.pos.e)) ||
		(r = parsevec(&line, scene->camera.ori.e)) ||
		(r = parsedouble(&line, &scene->camera.fov)))
		return r;
	if(scene->camera.fov <= 0.0f || scene->camera.fov > 180.0f)
		return 2;
	if(!(r = parsedouble(&line, &scene->camera.lensradius)))
	{
		if((r = parsedouble(&line, &scene->camera.focaldist)))
			return scene->camera.focaldist = 1.0f, r;
		if(scene->camera.focaldist <= 0.0f || scene->camera.lensradius < 0.0f)
			return 2;
	}
	scene->camera.ori = vec3norm(scene->camera.ori);
	scene->camera.rot = lookvector(scene->camera.ori);
	scene->camera.fov *= M_PI / 180.0f;

	line += countws(line);
	if(*line != '\n' && *(line) != 0)
		return 3;

	return 0;
}

int parsematerial(const char* line, t_scene* scene)
{
	t_material mat = {0};

	int r = 0;
	uint id = 0;
	if((r = parseuint(&line, &id)) ||
		(r = parsedouble(&line, &mat.metallic)) ||
		(r = parsedouble(&line, &mat.roughness)) ||
		(r = parsecolor(&line, mat.albedo.e)) ||
		(r = parsecolor(&line, mat.emit.e)))
		return r;

	if(!parseuint(&line, &mat.type))
	{
		if(mat.type == mat_checker)
		{
			if((r = parseuint(&line, &mat.checkermat)))
				return r;
			if(mat.checkermat >= scene->matvec.size)
				return 2;
		}
		else if(mat.type == mat_image)
		{
			uint tex = 0;
			if(!parseuint(&line, &tex) && tex && tex < scene->texvec.size)
				mat.talbedo = scene->texs + tex;
			if(!parseuint(&line, &tex) && tex && tex < scene->texvec.size)
				mat.troughness = scene->texs + tex;
		}
	}
	else
		mat.type = mat_solid;

	if(mat.metallic < 0.0f || mat.metallic > 1.0f ||
		mat.roughness < 0.0f || mat.roughness > 1.0f ||
		id >= MAX_MATERIALS || id == 0 || mat.type >= mat_max)
		return 2;

	mat.roughness *= mat.roughness; // square roughness

	while(id >= scene->matvec.size)
		if(!vecpush(&scene->matvec, scene->matvec.data))
			return 4;

	scene->mats[id] = mat;

	line += countws(line);
	if(*line != '\n' && *(line) != 0)
		return 3;

	return 0;
}

int parsetexture(const char* line, t_scene* scene)
{
	texture tex = {0};

	int r = 0;
	uint id = 0;
	textype type = 0;
	char* path = NULL;
	if((r = parseuint(&line, &id)) ||
		(r = parseuint(&line, &type)) ||
		(r = parsestring(&line, &path)))
	{
		free(path);
		return r;
	}

	if(!loadtexture(path, &tex, type))
	{
		free(path);
		return 5;
	}
	free(path);

	while(id >= scene->texvec.size)
		if(!vecpush(&scene->texvec, scene->texvec.data))
			return 4;

	scene->texs[id] = tex;

	line += countws(line);
	if(*line != '\n' && *(line) != 0)
		return 3;

	return 0;
}
