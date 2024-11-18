#include "pool_allocator.h"

#include "page_allocator.h"

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

static const size_t SIZE_LIST = 10;

struct Elem_list
{
    Elem_list* next;
    Elem_list* prev;
};

struct Pool_allocator
{
    Page_allocator* page_allocator;
    size_t chunk_size;

    Elem_list* chunk_free;
};

Pool_allocator* Pool_allocator_ctor(size_t chunk_size)
{
    Pool_allocator* pool_allocator = (Pool_allocator*)calloc(1, sizeof(Pool_allocator));
    
    pool_allocator->chunk_size = chunk_size;
    pool_allocator->page_allocator = Page_allocator_ctor((pool_allocator->chunk_size + sizeof(Elem_list)) * SIZE_LIST);

    pool_allocator->chunk_free = (Elem_list*)Page_alloc(pool_allocator->page_allocator);
    Elem_list* chunk_free_head = pool_allocator->chunk_free;

    for (size_t i = 1; i < SIZE_LIST; i++)
    {
        pool_allocator->chunk_free->next = (Elem_list*)((char*)pool_allocator->chunk_free
                                                        + pool_allocator->chunk_size + sizeof(Elem_list));
        pool_allocator->chunk_free = pool_allocator->chunk_free->next;
    }

    pool_allocator->chunk_free->next = chunk_free_head;
    Elem_list* chunk_free_tail = pool_allocator->chunk_free;

    for (size_t i = SIZE_LIST; i > 1; i--)
    {
        pool_allocator->chunk_free->prev = (Elem_list*)((char*)pool_allocator->chunk_free
                                                        - pool_allocator->chunk_size - sizeof(Elem_list));
        pool_allocator->chunk_free = pool_allocator->chunk_free->prev;
    }
    
    pool_allocator->chunk_free->prev = chunk_free_tail;

    return pool_allocator;
}

void Pool_allocator_dtor(Pool_allocator* pool_allocator)
{
    assert(pool_allocator != NULL);

    Page_allocator_dtor(pool_allocator->page_allocator);
    free(pool_allocator);
}

void* Pool_alloc(Pool_allocator* pool_allocator)
{
    assert(pool_allocator != NULL);

    //NOTE проверка на создание новой page

    Elem_list* chunk_free_memory = (Elem_list*)((char*)(pool_allocator->chunk_free) + sizeof(Elem_list));
    pool_allocator->chunk_free = pool_allocator->chunk_free->next;

    return chunk_free_memory;
}

void Pool_free(Pool_allocator* pool_allocator, void* elem_to_delete)
{
    assert(pool_allocator != NULL);

    Elem_list* chunk_to_delete = (Elem_list*)((char*)elem_to_delete - sizeof(Elem_list));

    chunk_to_delete->next->prev = chunk_to_delete->prev;
    chunk_to_delete->prev->next = chunk_to_delete->next;

    pool_allocator->chunk_free->next->next = chunk_to_delete;
    pool_allocator->chunk_free->prev = chunk_to_delete;
    chunk_to_delete->next = pool_allocator->chunk_free;
    chunk_to_delete->prev = pool_allocator->chunk_free->next;

    pool_allocator->chunk_free = (Elem_list*)elem_to_delete;
}