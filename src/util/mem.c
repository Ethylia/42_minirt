#include <stdlib.h>
#include <string.h>

void* memalloc(size_t size1, size_t size2, void* ptr)
{
	void* new = malloc(size1);
	if(!new)
	{
		free(ptr);
		return 0;
	}

	if(ptr)
		memcpy(new, ptr, size2);
	free(ptr);

	return new;
}

void intset(int *b, int c, size_t len)
{
	for(size_t i = 0; i < len; ++i)
		b[i] = c;
}
