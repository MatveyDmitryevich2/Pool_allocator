#include "pool_allocator.h"

#include "page_allocator.h"

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

static const size_t SIZE_LIST = 4;

struct Elem_list
{
    Elem_list* next;
    Elem_list* prev;
};

struct Pool_allocator
{
    Page_allocator* page_allocator;
    size_t chunk_size;

    Elem_list* first_chunk;

    Elem_list* chunk_free;
};

Pool_allocator* Pool_allocator_ctor(size_t chunk_size)
{
    Pool_allocator* pool_allocator = (Pool_allocator*)calloc(1, sizeof(Pool_allocator));
    
    pool_allocator->chunk_size = chunk_size;
    pool_allocator->page_allocator = Page_allocator_ctor((pool_allocator->chunk_size + sizeof(Elem_list)) * SIZE_LIST);

    pool_allocator->chunk_free = (Elem_list*)Page_alloc(pool_allocator->page_allocator);
    pool_allocator->first_chunk = pool_allocator->chunk_free;

    Elem_list* chunk_free_head = pool_allocator->chunk_free;
    fprintf(stderr, "HEAD:%p\n", pool_allocator->chunk_free);

    for (size_t i = 1; i < SIZE_LIST; i++)
    {
        pool_allocator->chunk_free->next = (Elem_list*)((char*)pool_allocator->chunk_free
                                                        + pool_allocator->chunk_size + sizeof(Elem_list));
        fprintf(stderr, "свой адрес:%p\n", pool_allocator->chunk_free);
        fprintf(stderr, "адрес следющего%p\n", pool_allocator->chunk_free->next);
        pool_allocator->chunk_free = pool_allocator->chunk_free->next;
    }

    pool_allocator->chunk_free->next = chunk_free_head;
    Elem_list* chunk_free_tail = pool_allocator->chunk_free;
    fprintf(stderr, "TAIL:%p\n", pool_allocator->chunk_free);
    fprintf(stderr, "TAIL->NEXT:%p\n", pool_allocator->chunk_free->next);

    fprintf(stderr, "-------------------------------------------------------\n");
    for (size_t i = SIZE_LIST; i > 1; i--)
    {
        pool_allocator->chunk_free->prev = (Elem_list*)((char*)pool_allocator->chunk_free
                                                        - pool_allocator->chunk_size - sizeof(Elem_list));
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
    Page_free(pool_allocator->page_allocator, pool_allocator->first_chunk);

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


    //fprintf(stderr, "chunk_to_delete = %p\nchunk_to_delete->next->prev = %p\n", chunk_to_delete, chunk_to_delete->next->prev);
    chunk_to_delete->next->prev = chunk_to_delete->prev;
    //fprintf(stderr, "chunk_to_delete = %p\nchunk_to_delete->next->prev = %p\n", chunk_to_delete, chunk_to_delete->next->prev);
    chunk_to_delete->prev->next = chunk_to_delete->next;


    fprintf(stderr, "\n\n\n\n\n\n\n\n\n-------------------------------------------\n");
    fprintf(stderr, "chunk_to_delete->prev = %p\n", chunk_to_delete->prev);
    fprintf(stderr, "pool_allocator->chunk_free = %p\n", pool_allocator->chunk_free);
    fprintf(stderr, "chunk_to_delete->next = %p\n", chunk_to_delete->next);
    fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);
    fprintf(stderr, "pool_allocator->chunk_free->next->prev = %p\n", pool_allocator->chunk_free->next->prev);
    fprintf(stderr, "chunk_to_delete = %p\n", chunk_to_delete);
    fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);

    chunk_to_delete->prev = pool_allocator->chunk_free;

    fprintf(stderr, "-------------------------------------------\n");
    fprintf(stderr, "chunk_to_delete->prev = %p\n", chunk_to_delete->prev);
    fprintf(stderr, "pool_allocator->chunk_free = %p\n", pool_allocator->chunk_free);
    fprintf(stderr, "chunk_to_delete->next = %p\n", chunk_to_delete->next);
    fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);
    fprintf(stderr, "pool_allocator->chunk_free->next->prev = %p\n", pool_allocator->chunk_free->next->prev);
    fprintf(stderr, "chunk_to_delete = %p\n", chunk_to_delete);
    fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);

    chunk_to_delete->next = pool_allocator->chunk_free->next;
    
    fprintf(stderr, "-------------------------------------------\n");
    fprintf(stderr, "chunk_to_delete->prev = %p\n", chunk_to_delete->prev);
    fprintf(stderr, "pool_allocator->chunk_free = %p\n", pool_allocator->chunk_free);
    fprintf(stderr, "chunk_to_delete->next = %p\n", chunk_to_delete->next);
    fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);
    fprintf(stderr, "pool_allocator->chunk_free->next->prev = %p\n", pool_allocator->chunk_free->next->prev);
    fprintf(stderr, "chunk_to_delete = %p\n", chunk_to_delete);
    fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);

    pool_allocator->chunk_free->next->prev = chunk_to_delete;
    
    fprintf(stderr, "-------------------------------------------\n");
    fprintf(stderr, "chunk_to_delete->prev = %p\n", chunk_to_delete->prev);
    fprintf(stderr, "pool_allocator->chunk_free = %p\n", pool_allocator->chunk_free);
    fprintf(stderr, "chunk_to_delete->next = %p\n", chunk_to_delete->next);
    fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);
    fprintf(stderr, "pool_allocator->chunk_free->next->prev = %p\n", pool_allocator->chunk_free->next->prev);
    fprintf(stderr, "chunk_to_delete = %p\n", chunk_to_delete);
    fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);

    pool_allocator->chunk_free->next = chunk_to_delete;
    
    fprintf(stderr, "-------------------------------------------\n");
    fprintf(stderr, "chunk_to_delete->prev = %p\n", chunk_to_delete->prev);
    fprintf(stderr, "pool_allocator->chunk_free = %p\n", pool_allocator->chunk_free);
    fprintf(stderr, "chunk_to_delete->next = %p\n", chunk_to_delete->next);
    fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);
    fprintf(stderr, "pool_allocator->chunk_free->next->prev = %p\n", pool_allocator->chunk_free->next->prev);
    fprintf(stderr, "chunk_to_delete = %p\n", chunk_to_delete);
    fprintf(stderr, "pool_allocator->chunk_free->next = %p\n", pool_allocator->chunk_free->next);

    //pool_allocator->chunk_free = (Elem_list*)elem_to_delete; ЧО ЗА ХУЙНЯ
}