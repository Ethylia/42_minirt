#ifndef MATERIAL_H
#define MATERIAL_H

#include "math/vec3.h"

typedef struct s_material {
	float metallic;
	float roughness;
	vec3 albedo;
	vec3 emit;
} t_material;

#endif


