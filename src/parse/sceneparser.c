#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "util/util.h"
#include "math/mat3.h"
#include "math/vec3.h"
#include "obj/scene.h"
#include "parseobj.h"

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
	scene->camera.ori = vec3norm(scene->camera.ori);
	scene->camera.rot = lookvector(scene->camera.ori);
	scene->camera.fov *= M_PI / 180.0f;

	line += countws(line);
	if(*line != '\n' && *(line) != 0)
		return 3;

	return 0;
}
