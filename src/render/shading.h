#ifndef SHADING_H
#define SHADING_H

#include "obj/scene.h"
#include "render/context.h"
#include "render/raycast.h"

vec3 shade(t_ray ray, const t_scene *s);

#endif
