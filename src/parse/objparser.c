#include <stdlib.h>
#include "parse/parseobj.h"
#include "util/util.h"
#include "math/vec3.h"
#include "obj/scene.h"
#include "def.h"

int parsesphere(const char* line, t_scene* scene)
{
	t_obj* sphere = sceneaddobj(scene);
	if(!sphere)
		return 4;
	sphere->type = e_sphere;

	int r = 0;
	if((r = parsevec(&line, sphere->sphere.pos.e)) ||
		(r = parsedouble(&line, &sphere->sphere.rad)) ||
		(r = parsecolor(&line, sphere->sphere.color.e)) ||
		(r = parsecolor(&line, sphere->emit.e)))
		return r;

	if(sphere->sphere.rad <= 0.0f)
		return 2;
	
	sphere->sphere.rad /= 2.0f;
	
	line += countws(line);
	if(*line != '\n' && *(line) != 0)
		return 3;

	return 0;
}

int parseplane(const char* line, t_scene* scene)
{
	t_obj* plane;

	plane = sceneaddobj(scene);
	if(!plane)
		return 4;
	plane->type = e_plane;

	int r = 0;
	if((r = parsevec(&line, plane->plane.pos.e)) ||
		(r = parsevec(&line, plane->plane.ori.e)) ||
		(r = parsecolor(&line, plane->plane.color.e)) ||
		(r = parsecolor(&line, plane->emit.e)))
		return 3;

	line += countws(line);
	if(*line != '\n' && *(line) != 0)
		return 3;

	plane->plane.ori = vec3norm(plane->plane.ori);
	return 0;
}

int parsecylinder(const char* line, t_scene* scene)
{
	t_obj* cylinder;

	cylinder = sceneaddobj(scene);
	if(!cylinder)
		return 4;
	cylinder->type = e_cylinder;

	int r = 0;
	if((r = parsevec(&line, cylinder->cylinder.pos.e)) ||
		(r = parsevec(&line, cylinder->cylinder.axis.e)) ||
		(r = parsedouble(&line, &cylinder->cylinder.rad)) ||
		(r = parsedouble(&line, &cylinder->cylinder.height)) ||
		(r = parsecolor(&line, cylinder->cylinder.color.e)) ||
		(r = parsecolor(&line, cylinder->emit.e)))
		return 3;

	if(cylinder->cylinder.rad <= 0.0f || cylinder->cylinder.height <= 0.0f)
		return 2;

	line += countws(line);
	if(*line != '\n' && *(line) != 0)
		return 3;

	cylinder->cylinder.axis = vec3norm(cylinder->cylinder.axis);
	return 0;
}
