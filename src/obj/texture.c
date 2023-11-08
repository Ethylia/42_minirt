#include <stdlib.h>

#include "MLX42/MLX42.h"

#include "obj/material.h"
#include "math/vec2.h"

int loadtexture(const char* texfile, texture* tex, textype type)
{
	mlx_texture_t* mlx_tex = mlx_load_png(texfile);
	if(!mlx_tex)
		return 0;

	if(mlx_tex->bytes_per_pixel != 4)
	{
		mlx_delete_texture(mlx_tex);
		return 0;
	}
	tex->width = mlx_tex->width;
	tex->height = mlx_tex->height;
	if(type == tex_float)
		tex->dataf = malloc(tex->width * tex->height * sizeof(float));
	else
		tex->data = malloc(tex->width * tex->height * sizeof(vec3));
	if(!tex->data)
	{
		mlx_delete_texture(mlx_tex);
		return 0;
	}
	if(type == tex_float)
		for(size_t i = 0; i < tex->width * tex->height; ++i)
			tex->dataf[i] = mlx_tex->pixels[i * mlx_tex->bytes_per_pixel] / 255.0f;
	else
	{
		for(size_t i = 0; i < tex->width * tex->height; ++i)
		{
			tex->data[i].x = mlx_tex->pixels[i * 4 + 0] / 255.0f;
			tex->data[i].y = mlx_tex->pixels[i * 4 + 1] / 255.0f;
			tex->data[i].z = mlx_tex->pixels[i * 4 + 2] / 255.0f;
		}
	}

	mlx_delete_texture(mlx_tex);
	return 1;
}

vec3 textureuv(const texture* tex, const vec2 uv)
{
	// linear interpolation
	const float x = uv.x * tex->width;
	const float y = uv.y * tex->height;
	const size_t x0 = (size_t)x % tex->width;
	const size_t y0 = (size_t)y % tex->height;
	const size_t x1 = (x0 + 1) % tex->width;
	const size_t y1 = (y0 + 1) % tex->height;
	const float xfrac = x - (size_t)x;
	const float yfrac = y - (size_t)y;
	const vec3* c00 = &tex->data[y0 * tex->width + x0];
	const vec3* c01 = &tex->data[y0 * tex->width + x1];
	const vec3* c10 = &tex->data[y1 * tex->width + x0];
	const vec3* c11 = &tex->data[y1 * tex->width + x1];
	return vec3add(vec3add(vec3add(vec3scale(*c00, (1.0f - xfrac) * (1.0f - yfrac)),
		vec3scale(*c01, xfrac * (1.0f - yfrac))),
		vec3scale(*c10, (1.0f - xfrac) * yfrac)),
		vec3scale(*c11, xfrac * yfrac));
}

float textureuvf(const texture* tex, const vec2 uv)
{
	// linear interpolation
	const float x = uv.x * tex->width;
	const float y = uv.y * tex->height;
	const size_t x0 = (size_t)x % tex->width;
	const size_t y0 = (size_t)y % tex->height;
	const size_t x1 = (x0 + 1) % tex->width;
	const size_t y1 = (y0 + 1) % tex->height;
	const float xfrac = x - (size_t)x;
	const float yfrac = y - (size_t)y;
	const float c00 = tex->dataf[y0 * tex->width + x0];
	const float c01 = tex->dataf[y0 * tex->width + x1];
	const float c10 = tex->dataf[y1 * tex->width + x0];
	const float c11 = tex->dataf[y1 * tex->width + x1];
	return c00 * (1.0f - xfrac) * (1.0f - yfrac) +
		c01 * xfrac * (1.0f - yfrac) +
		c10 * (1.0f - xfrac) * yfrac +
		c11 * xfrac * yfrac;
}

void freetexture(texture* tex)
{
	free(tex->data);
	tex->data = 0;
}
