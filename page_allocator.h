#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H

#include <stddef.h>

struct Page_allocator;

Page_allocator* Page_allocator_ctor(size_t page_size_bytes);
void Page_allocator_dtor(Page_allocator* page_allocator);
void* Page_alloc(Page_allocator* page_allocator);
size_t Get_n_pages(Page_allocator* page_allocator);
size_t Get_page_size(Page_allocator* page_allocator);
void Page_free(Page_allocator* page_allocator, void* page);
void* Next_page(void* page);
void* Prev_page(void* page);

#endif//PAGE_ALLOCATOR_H