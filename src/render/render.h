#ifndef RENDER_H
#define RENDER_H

#include "render/context.h"

typedef struct s_region
{
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	unsigned int imgid;
} t_region;

void renderregion(t_context* context, const t_region region);
void render(t_context* context);

#endif
