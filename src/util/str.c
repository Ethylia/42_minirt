#include <stddef.h>

void* memcopy(void* dest, const void* src, size_t size)
{
	for(size_t i = 0; i < size; ++i)
		((char*)dest)[i] = ((char*)src)[i];
	return dest;
}

size_t countws(char const* str)
{
	for(size_t i = 0;; ++i)
		if(str[i] && str[i] != ' ' && str[i] != '\t')
			return i;
	return 0;
}
