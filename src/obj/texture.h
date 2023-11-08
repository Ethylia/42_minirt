#ifndef TEXTURE_H
#define TEXTURE_H

#include "math/vec2.h"

struct texture;

int loadtexture(const char* texfile, texture* tex, textype type);
vec3 textureuv(const texture* tex, const vec2 uv);
float textureuvf(const texture* tex, const vec2 uv);
void freetexture(texture* tex);

#endif
