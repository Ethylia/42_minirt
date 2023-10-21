#ifndef SHADING_H
#define SHADING_H

#include "obj/scene.h"
#include "render/context.h"
#include "render/raycast.h"

vec3 shade(t_obj *obj, vec3 hit, const t_scene *s, const vec3 ray);

#endif
