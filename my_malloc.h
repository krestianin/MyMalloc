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

// MemoryBlock* root = NULL; // Root of the AVL tree


void initializeMemory();
void* myMalloc(size_t size);
void myFree(void* ptr);
void cleanupMemory(); 

#endif 