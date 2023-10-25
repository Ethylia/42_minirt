#include <stdlib.h>

typedef struct s_vector {
	void* data;
	size_t size;
	size_t capacity;
	size_t elemsize;
} vector;

int vecinit(vector* v, size_t elemsize, size_t capacity);
int vecpush(vector* v, void* data);
void vecfree(vector* v);
