#include <stddef.h>
#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#define MEMORY_SIZE 1024

struct MemoryBlock {
    size_t size;
    int status; // 0 for free, 1 for allocated
    struct MemoryBlock* next; // Pointer to the next free block
    void* memory;
};

void initializeMemory();
void* myMalloc(size_t size);
void myFree(void* ptr);
void cleanupMemory(); 

#endif 