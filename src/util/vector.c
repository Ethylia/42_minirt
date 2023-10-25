#include "vector.h"

#include <stdlib.h>
#include <string.h>

int vecinit(vector* v, size_t elemsize, size_t capacity)
{
	v->elemsize = elemsize;
	v->capacity = capacity;
	v->size = 0;
	v->data = malloc(elemsize * capacity);
	return !!v->data;
}

int vecpush(vector* v, void* data)
{
	if(v->size >= v->capacity)
	{
		v->capacity *= 2;
		void* tmp = realloc(v->data, v->elemsize * v->capacity);
		if(!tmp)
			return 0;
		else
			v->data = tmp;
	}
	memcpy((char*)v->data + v->size * v->elemsize, data, v->elemsize);
	++v->size;
	return 1;
}

void vecfree(vector* v)
{
	free(v->data);
	v->data = 0;
	v->capacity = 0;
	v->size = 0;
}
