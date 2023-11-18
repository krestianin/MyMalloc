// #include <stddef.h>
// #include "my_malloc.h"
// #include "stdio.h"



// char memory_pool[MEMORY_SIZE];
// struct MemoryBlock* free_list = NULL;

// void initializeMemory() {
//     // Initialize a single memory block that covers the entire pool
//     struct MemoryBlock* initial_block = (struct MemoryBlock*)memory_pool;
//     initial_block->size = MEMORY_SIZE;
//     initial_block->status = 0; // Free
//     initial_block->next = NULL;
//     initial_block->memory = memory_pool + sizeof(struct MemoryBlock);
    
//     // Set the initial block as the head of the free list
//     free_list = initial_block;
// }

// void* myMalloc(size_t size) {
//     struct MemoryBlock* current_block = free_list;
//     struct MemoryBlock* prev_block = NULL;

//     // Find a suitable free block from the free list
//     while (current_block && (current_block->status == 1 || current_block->size < size)) {
//         prev_block = current_block;
//         current_block = current_block->next;
//     }

//     if (!current_block) {
//         // No suitable block found
//         return NULL;
//     }

//     // If the block is larger than needed, split it
//     if (current_block->size > size + sizeof(struct MemoryBlock)) {
//         struct MemoryBlock* next_block = (struct MemoryBlock*)((char*)current_block + size + sizeof(struct MemoryBlock));
//         next_block->size = current_block->size - size - sizeof(struct MemoryBlock);
//         next_block->status = 0; // Free
//         next_block->next = current_block->next;
//         next_block->memory = (char*)next_block + sizeof(struct MemoryBlock);

//         current_block->size = size;
//         current_block->next = next_block;
//     }

//     // Update the block status and remove it from the free list
//     if (prev_block) {
//         prev_block->next = current_block->next;
//     } else {
//         free_list = current_block->next;
//     }
//     current_block->status = 1; // Allocated

//     return current_block->memory;
// }

// void myFree(void* ptr) {
//     if (!ptr) {
//         // Nothing to free
//         return;
//     }

//     // Mark the corresponding memory block as free
//     struct MemoryBlock* block_to_free = (struct MemoryBlock*)((char*)ptr - sizeof(struct MemoryBlock));
//     block_to_free->status = 0;

//     // Push the block onto the free list
//     block_to_free->next = free_list;
//     free_list = block_to_free;
// }
#include "my_malloc.h"
#include <stdlib.h>
#include <stdio.h>
// #include <climits>

static char* memory_pool = NULL;
static char* stack_top = NULL;

void initializeMemory() {
    memory_pool = (char*)malloc(MEMORY_SIZE);
    if (memory_pool == NULL) {
        fprintf(stderr, "Error allocating memory pool\n");
        exit(EXIT_FAILURE);
    }
    stack_top = memory_pool;
}

void* myMalloc(size_t size) {
    if (size > 65535 - sizeof(unsigned short)) {
        // Block size is too large to be tracked with an unsigned short
        return NULL;
    }

    unsigned short total_size = (unsigned short)size + sizeof(unsigned short);
    if (stack_top + total_size > memory_pool + MEMORY_SIZE) {
        return NULL;
    }

    *((unsigned short*)stack_top) = (unsigned short)size;
    void* allocated_memory = stack_top + sizeof(unsigned short);

    stack_top += total_size;

    return allocated_memory;
}

void myFree(void* ptr) {
    if (!ptr) {
        return;
    }

    unsigned short block_size = *((unsigned short*)((char*)ptr - sizeof(unsigned short)));
    char* expected_block_start = (char*)ptr - sizeof(unsigned short);

    if (expected_block_start != stack_top - block_size - sizeof(unsigned short)) {
        printf("Error: Memory block not at stack top. Cannot free.\n");
        return;
    }

    stack_top = expected_block_start;
}


void cleanupMemory() {
    free(memory_pool);  // Free the allocated memory pool
}