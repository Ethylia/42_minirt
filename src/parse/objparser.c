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
		(r = parsedouble(&line, &sphere->sphere.rad)))
		return r;

	if(sphere->sphere.rad <= 0.0f)
		return 2;

	uint matid = 0;
	if(!(r = parseuint(&line, &matid)))
		if(matid >= scene->matcount)
			return 2;
	sphere->mat = scene->mats + matid;

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
		(r = parsevec(&line, plane->plane.ori.e)))
		return 3;

	uint matid = 0;
	if(!(r = parseuint(&line, &matid)))
		if(matid >= scene->matcount)
			return 2;
	plane->mat = scene->mats + matid;

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
		(r = parsedouble(&line, &cylinder->cylinder.height)))
		return 3;

	if(cylinder->cylinder.rad <= 0.0f || cylinder->cylinder.height <= 0.0f)
		return 2;

	uint matid = 0;
	if(!(r = parseuint(&line, &matid)))
		if(matid >= scene->matcount)
			return 2;
	cylinder->mat = scene->mats + matid;

	line += countws(line);
	if(*line != '\n' && *(line) != 0)
		return 3;

	cylinder->cylinder.axis = vec3norm(cylinder->cylinder.axis);
	return 0;
}
