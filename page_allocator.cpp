#include "page_allocator.h"

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

struct Page
{
    Page* next_page;
    Page* prev_page;
};

struct Page_allocator
{
    size_t page_size_bytes;
    size_t n_pages; // не учитывает фиктивную страницу

    Page* fake_page;
};

Page_allocator* Page_allocator_ctor(size_t page_size_bytes)
{
    Page_allocator* page_allocator = (Page_allocator*)calloc(1, sizeof(Page_allocator));

    page_allocator->fake_page = (Page*)calloc(1, sizeof(Page) + page_allocator->page_size_bytes);
    page_allocator->fake_page->next_page = page_allocator->fake_page;
    page_allocator->fake_page->prev_page = page_allocator->fake_page;

    page_allocator->page_size_bytes = page_size_bytes;
    page_allocator->n_pages = 0; 

    return page_allocator;
}

void Page_allocator_dtor(Page_allocator* page_allocator)
{
    assert(page_allocator != NULL);

    free(page_allocator->fake_page);
    free(page_allocator);
}

void* Page_alloc(Page_allocator* page_allocator)
{
    assert(page_allocator != NULL);

    Page* page = (Page*)calloc(1, sizeof(Page) + page_allocator->page_size_bytes);

    page->next_page = page_allocator->fake_page;
    page->prev_page = page_allocator->fake_page->prev_page;
    page_allocator->fake_page->prev_page->next_page = page;
    page_allocator->fake_page->prev_page = page;
    
    page_allocator->n_pages++;

    return (void*)((char*)page + sizeof(Page));
}

void Page_free(Page_allocator* page_allocator, void* page)
{
    assert(page_allocator != NULL);
    assert(page != NULL);

    Page* page_header = (Page*)((char*)page - sizeof(Page));

    page_header->next_page->prev_page = page_header->prev_page;
    page_header->prev_page->next_page = page_header->next_page;

    free(page_header);

    page_allocator->n_pages--;
}

size_t Get_n_pages(Page_allocator* page_allocator)
{
    assert(page_allocator != NULL);

    return page_allocator->n_pages;
}

size_t Get_page_size(Page_allocator* page_allocator)
{
    assert(page_allocator != NULL);

    return page_allocator->page_size_bytes;
}
