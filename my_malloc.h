#include <stddef.h>
#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#define MEMORY_SIZE 1024

typedef struct MemoryBlock {
    void* ptr;
    size_t size;
    struct MemoryBlock* left;
    struct MemoryBlock* right;
    int height;
} MemoryBlock;


void initializeMemory();
void* my_malloc(size_t size);
void my_free(void* ptr);
void cleanupMemory(); 

#endif 