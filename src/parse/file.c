#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/util.h"
#include "parse/parseobj.h"
#include "obj/scene.h"

#define ALLOC_SIZE 1024

static char* loadfile(const char* path)
{
	const int fd = open(path, O_RDONLY);

	if(fd < 0)
		return 0;

	char* data = memalloc(ALLOC_SIZE, 0, 0);
	if(!data)
	{
		close(fd);
		return 0;
	}

	size_t size = 0;
	ssize_t r;
	while(data)
	{
		r = read(fd, data + size, ALLOC_SIZE);
		if(r == ALLOC_SIZE)
		{ // If we read the full buffer, allocate more memory and continue
			size += ALLOC_SIZE;
			if(!(data = memalloc(size + ALLOC_SIZE, size, data)))
				break;
		}
		else // If we read less than the full buffer, we're done
			break;
	}
	close(fd);
	if(r < 0 || !data)
	{
		free(data);
		return 0;
	}
	data[size + r] = 0;

	return data;
}

static int parseline(const char* line, t_scene* scene)
{
	const char* const id[] =
		{
			ID_A_LIGHT, ID_SAMPLES, ID_CAMERA,
			ID_SPHERE, ID_PLANE, ID_CYLINDER, ID_MATERIAL, ID_TEXTURE
		};
	static int (* const funcs[])(const char*, t_scene*) =
		{
			parseambient, parsesettings, parsecamera, parsesphere,
			parseplane, parsecylinder, parsematerial, parsetexture
		};

	line += countws(line);
	if(*line == '#') // masterfully crafted comment implementation
		return 0;

	for(size_t i = 0; i < sizeof(id) / sizeof(*id); ++i)
	{
		if(strncmp(line, id[i], strlen(id[i])) == 0
			&& (line[strlen(id[i])] == ' '
				|| line[strlen(id[i])] == '\t'))
			return (funcs[i](line + strlen(id[i]) + 1
				+ countws(line + strlen(id[i]) + 1), scene));
	}

	return 1;
}

static int printerror(char* str, size_t pos, size_t line, int err)
{
	const char* const	errstr[] = {
		"Invalid type", "Invalid value", "Invalid format" };
	const int			len = strchr(str + pos, '\n') - str - pos;

	switch(err)
	{
	case 4:
		printf("Memory error\n");
		break;
	case 5:
		printf("File error\n");
		break;
	default:
		printf("%s on line %zu\n->\t%.*s\n", errstr[err - 1], line, len, str + pos);
		free(str);
		break;
	}

	return err;
}

int	loadscene(const char* path, t_scene* scene)
{
	char* file = loadfile(path);
	if(!file)
		return 5;
	int r = 0;
	size_t i = 0;
	size_t line = 1;
	while(file[i])
	{
		if((r = parseline(file + i, scene)))
			return printerror(file, i, line, r);
		while(file[i] && file[i] != '\n')
			++i;
		++i;
		++line;
	}
	free(file);
	return 0;
}
