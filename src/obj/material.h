#ifndef MATERIAL_H
#define MATERIAL_H

#include "math/vec3.h"
#include "def.h"

typedef enum e_textype
{
	tex_rgb = 0,
	tex_float = 1
} textype;

typedef struct s_texture
{
	textype type;
	int width;
	int height;
	union
	{
		vec3* data;
		float* dataf;
	};
} texture;

typedef enum e_mattype
{
	mat_image,
	mat_solid,
	mat_checker,
	mat_max
} mattype;

typedef struct s_material {
	mattype type;
	uint checkermat;
	float metallic;
	float roughness;
	texture* talbedo;
	texture* troughness;
	vec3 albedo;
	vec3 emit;
} t_material;

#endif


