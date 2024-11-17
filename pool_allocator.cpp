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

    for (size_t i = 1; i < SIZE_LIST; i++)
    {
        pool_allocator->chunk_free->next = (Elem_list*)((char*)pool_allocator->chunk_free + pool_allocator->chunk_size + sizeof(Elem_list));
        pool_allocator->chunk_free = pool_allocator->chunk_free->next;
    }

    for (size_t i = SIZE_LIST; i > 1; i--)
    {
        pool_allocator->chunk_free->prev = (Elem_list*)((char*)pool_allocator->chunk_free - pool_allocator->chunk_size - sizeof(Elem_list));
        pool_allocator->chunk_free = pool_allocator->chunk_free->prev;
    }
}

void* Pool_alloc(Pool_allocator* pool_allocator)
{
    assert(pool_allocator != NULL);
    if (pool_allocator->chunk_free == (char*)pool_allocator->chunk_free + )//проверка на конец
    //если что реалок
    // ну здесь понятно че
    // не забыть про ретурн
}

void Pool_allocator_dtor(Pool_allocator* pool_allocator)
{
    assert(pool_allocator != NULL);

    Page_allocator_dtor(pool_allocator->page_allocator);
    free(pool_allocator);
}