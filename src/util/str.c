#include <stddef.h>

size_t countws(char const* str)
{
	for(size_t i = 0;; ++i)
		if(str[i] && str[i] != ' ' && str[i] != '\t')
			return i;
	return 0;
}
