#ifndef laplas_memory_h
#define laplas_memory_h

#include "common.h"

#define ALLOCATE(type, count) \
    reallocate(NULL, 0, sizeof(type) * (count))

#define FREE(type, pointer) \
    reallocate(pointer, sizeof(type), 0)

#define REALLOCATE_ARRAY(type, pointer, oldCount, newCount) \
    reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, count) \
    reallocate(pointer, sizeof(type) * (count), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif