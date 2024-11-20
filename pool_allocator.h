#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <stdio.h>

#define USE_CALLOC

struct Pool_allocator;

Pool_allocator* Pool_allocator_ctor(size_t chunk_size);
void Pool_allocator_dtor(Pool_allocator* pool_allocator);
void* Pool_alloc(Pool_allocator* pool_allocator);
void Pool_free(Pool_allocator* pool_allocator, void* elem_to_delete);
void* Binding_free_chunk(Pool_allocator* pool_allocator);

#endif // POOL_ALLOCATOR_H