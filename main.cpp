#include <stdio.h>

#include "pool_allocator.h"
#include "page_allocator.h"

int main()
{
    Pool_allocator* pool_allocator = Pool_allocator_ctor(128);

    char* memory3 = (char*)Pool_alloc(pool_allocator);
    char* memory1 = (char*)Pool_alloc(pool_allocator);
    // Pool_free(pool_allocator, memory1);
    //Pool_free(pool_allocator, memory3);
    //char* memory2 = (char*)Pool_alloc(pool_allocator);
    char* memory5 = (char*)Pool_alloc(pool_allocator);
    // Pool_free(pool_allocator, memory2);
    // char* memory7 = (char*)Pool_alloc(pool_allocator);
    // char* memory8 = (char*)Pool_alloc(pool_allocator);
    // char* memory9 = (char*)Pool_alloc(pool_allocator);
    // char* memory10 = (char*)Pool_alloc(pool_allocator);
    // char* memory11 = (char*)Pool_alloc(pool_allocator);
    // Pool_free(pool_allocator, memory5);



    Pool_allocator_dtor(pool_allocator);



    // Page_allocator* page_allocator = Page_allocator_ctor(128);

    // char* memory3 = (char*)Page_alloc(page_allocator);
    // char* memory1 = (char*)Page_alloc(page_allocator);
    // char* memory2 = (char*)Page_alloc(page_allocator);
    // Page_free(page_allocator, memory1);
    // Page_free(page_allocator, memory2);
    // Page_free(page_allocator, memory3);
    // char* memory4 = (char*)Page_alloc(page_allocator);
    // char* memory5 = (char*)Page_alloc(page_allocator);
    // Page_free(page_allocator, memory5);
    // Page_free(page_allocator, memory4);
    // char* memory6 = (char*)Page_alloc(page_allocator);
    // Page_free(page_allocator, memory6);
    // char* memory7 = (char*)Page_alloc(page_allocator);
    // Page_free(page_allocator, memory7);

    // Page_allocator_dtor(page_allocator);
}