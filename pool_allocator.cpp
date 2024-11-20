#include "pool_allocator.h"

#include "page_allocator.h"

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

static const size_t SIZE_LIST = 3;

struct Pool_header
{
    Pool_header* next;
    Pool_header* prev;
};

struct Pool_allocator
{
    Page_allocator* page_allocator;
    size_t chunk_size;

    Pool_header* first_chunk;

    Pool_header* chunk_free;
};

Pool_allocator* Pool_allocator_ctor(size_t chunk_size)
{
    Pool_allocator* pool_allocator = (Pool_allocator*)calloc(1, sizeof(Pool_allocator));
    assert(pool_allocator != NULL);
    
    pool_allocator->chunk_size = chunk_size;
    pool_allocator->page_allocator = Page_allocator_ctor((pool_allocator->chunk_size + sizeof(Pool_header)) * SIZE_LIST);

    pool_allocator->chunk_free = (Pool_header*)Page_alloc(pool_allocator->page_allocator);
    pool_allocator->first_chunk = pool_allocator->chunk_free;

    Pool_header* chunk_free_head = pool_allocator->chunk_free;
    fprintf(stderr, "HEAD:%p\n", pool_allocator->chunk_free);

    for (size_t i = 1; i < SIZE_LIST; i++)
    {
        pool_allocator->chunk_free->next = (Pool_header*)((char*)pool_allocator->chunk_free
                                                        + pool_allocator->chunk_size + sizeof(Pool_header));
        fprintf(stderr, "свой адрес:%p\n", pool_allocator->chunk_free);
        fprintf(stderr, "адрес следющего%p\n", pool_allocator->chunk_free->next);
        pool_allocator->chunk_free = pool_allocator->chunk_free->next;
    }

    pool_allocator->chunk_free->next = chunk_free_head;
    Pool_header* chunk_free_tail = pool_allocator->chunk_free;
    fprintf(stderr, "TAIL:%p\n", pool_allocator->chunk_free);
    fprintf(stderr, "TAIL->NEXT:%p\n", pool_allocator->chunk_free->next);

    fprintf(stderr, "-------------------------------------------------------\n");
    for (size_t i = SIZE_LIST; i > 1; i--)
    {
        pool_allocator->chunk_free->prev = (Pool_header*)((char*)pool_allocator->chunk_free
                                                        - pool_allocator->chunk_size - sizeof(Pool_header));
        fprintf(stderr, "свой адрес:%p\n", pool_allocator->chunk_free);
        fprintf(stderr, "адрес предидущего%p\n", pool_allocator->chunk_free->prev);
        pool_allocator->chunk_free = pool_allocator->chunk_free->prev;
    }
    
    pool_allocator->chunk_free->prev = chunk_free_tail;
    fprintf(stderr, "HEAD->PREV:%p\n", pool_allocator->chunk_free->prev);

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

    if (pool_allocator->chunk_free->next == pool_allocator->chunk_free)
    {
        fprintf(stderr, "\n------------------------СРАБОТАЛА ПРОВЕРКА-----------------\n");
        Binding_free_chunk(pool_allocator);

        pool_allocator->chunk_free = (Pool_header*)Page_alloc(pool_allocator->page_allocator);
        pool_allocator->first_chunk = pool_allocator->chunk_free;

        Pool_header* chunk_free_head = pool_allocator->chunk_free;
        fprintf(stderr, "HEAD:%p\n", pool_allocator->chunk_free);

        for (size_t i = 1; i < SIZE_LIST; i++)
        {
            pool_allocator->chunk_free->next = (Pool_header*)((char*)pool_allocator->chunk_free
                                                            + pool_allocator->chunk_size + sizeof(Pool_header));
            fprintf(stderr, "свой адрес:%p\n", pool_allocator->chunk_free);
            fprintf(stderr, "адрес следющего%p\n", pool_allocator->chunk_free->next);
            pool_allocator->chunk_free = pool_allocator->chunk_free->next;
        }

        pool_allocator->chunk_free->next = chunk_free_head;
        Pool_header* chunk_free_tail = pool_allocator->chunk_free;
        fprintf(stderr, "TAIL:%p\n", pool_allocator->chunk_free);
        fprintf(stderr, "TAIL->NEXT:%p\n", pool_allocator->chunk_free->next);

        fprintf(stderr, "-------------------------------------------------------\n");
        for (size_t i = SIZE_LIST; i > 1; i--)
        {
            pool_allocator->chunk_free->prev = (Pool_header*)((char*)pool_allocator->chunk_free
                                                            - pool_allocator->chunk_size - sizeof(Pool_header));
            fprintf(stderr, "свой адрес:%p\n", pool_allocator->chunk_free);
            fprintf(stderr, "адрес предидущего%p\n", pool_allocator->chunk_free->prev);
            pool_allocator->chunk_free = pool_allocator->chunk_free->prev;
        }
        
        pool_allocator->chunk_free->prev = chunk_free_tail;
        fprintf(stderr, "HEAD->PREV:%p\n", pool_allocator->chunk_free->prev);

        return (Pool_header*)((char*)(pool_allocator->chunk_free) + sizeof(Pool_header));
    }

    Pool_header* chunk_free_memory = (Pool_header*)Binding_free_chunk(pool_allocator);

    return chunk_free_memory;
}

void* Binding_free_chunk(Pool_allocator* pool_allocator)
{
    // fprintf(stderr, "\n\n\n\n\n\n------------------------ДО-------------------\n");
    // fprintf(stderr, "pool_allocator->chunk_free = %p\n", pool_allocator->chunk_free);
    // fprintf(stderr, "pool_allocator->chunk_free->prev = %p\n", pool_allocator->chunk_free->prev);
    // fprintf(stderr, "pool_allocator->chunk_free->prev->next = %p\n", pool_allocator->chunk_free->prev->next);
    // fprintf(stderr, "pool_allocator->chunk_free->next->prev = %p\n", pool_allocator->chunk_free->next->prev);
    // fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);
    pool_allocator->chunk_free->prev->next = pool_allocator->chunk_free->next;
    // fprintf(stderr, "\n\n\n\n\n\n-----------------------ПЕРВАЯ ХУИТА--------------------\n");
    // fprintf(stderr, "pool_allocator->chunk_free = %p\n", pool_allocator->chunk_free);
    // fprintf(stderr, "pool_allocator->chunk_free->prev = %p\n", pool_allocator->chunk_free->prev);
    // fprintf(stderr, "pool_allocator->chunk_free->prev->next = %p\n", pool_allocator->chunk_free->prev->next);
    // fprintf(stderr, "pool_allocator->chunk_free->next->prev = %p\n", pool_allocator->chunk_free->next->prev);
    // fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);
    pool_allocator->chunk_free->next->prev = pool_allocator->chunk_free->prev;
    // fprintf(stderr, "\n\n\n\n\n\n-----------------------ВТОРАЯ ХУИТА--------------------\n");
    // fprintf(stderr, "pool_allocator->chunk_free = %p\n", pool_allocator->chunk_free);
    // fprintf(stderr, "pool_allocator->chunk_free->prev = %p\n", pool_allocator->chunk_free->prev);
    // fprintf(stderr, "pool_allocator->chunk_free->prev->next = %p\n", pool_allocator->chunk_free->prev->next);
    // fprintf(stderr, "pool_allocator->chunk_free->next->prev = %p\n", pool_allocator->chunk_free->next->prev);
    // fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);

    Pool_header* chunk_free_memory = (Pool_header*)((char*)(pool_allocator->chunk_free) + sizeof(Pool_header));

    pool_allocator->chunk_free->prev = NULL; //зануление prev для прошлого фри
    Pool_header** buffer = &(pool_allocator->chunk_free->next);
    pool_allocator->chunk_free = pool_allocator->chunk_free->next;
    *buffer = NULL; //зануление next для прошлого фри

    return chunk_free_memory;
}

void Pool_free(Pool_allocator* pool_allocator, void* elem_to_delete)
{
    assert(pool_allocator != NULL);
    assert(elem_to_delete != NULL);

    Pool_header* chunk_to_delete = (Pool_header*)((char*)elem_to_delete - sizeof(Pool_header));

    chunk_to_delete->prev = pool_allocator->chunk_free->prev;
    pool_allocator->chunk_free->prev->next = chunk_to_delete;
    pool_allocator->chunk_free->prev = chunk_to_delete;
    chunk_to_delete->next = pool_allocator->chunk_free;

    pool_allocator->chunk_free = chunk_to_delete;
}