#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

size_t countws(const char* str);

void* memalloc(size_t size1, size_t size2, void* ptr);
void intset(int* b, int c, size_t len);

#endif
