#include <stdlib.h>

// operator new and delete implementations

void* operator new(size_t objsize) { 
    return malloc(objsize); 
} 

void operator delete(void* obj) { 
    free(obj); 
} 

void * operator new[](size_t size) { 
    return malloc(size); 
} 

void operator delete[](void * ptr) 
{ 
    free(ptr); 
} 
