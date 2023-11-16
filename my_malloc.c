#include <stddef.h>
#include "my_malloc.h"
#include "stdio.h"



char memory_pool[MEMORY_SIZE];
struct MemoryBlock* memory_blocks;

void initializeMemory() {
    // Initialize a single memory block that covers the entire pool
    memory_blocks = (struct MemoryBlock*)memory_pool;
    memory_blocks->size = MEMORY_SIZE;
    memory_blocks->status = 0; // Free
    memory_blocks->memory = memory_pool + sizeof(struct MemoryBlock);
    // printf("Hello!");  
}

void* myMalloc(size_t size) {
    struct MemoryBlock* current_block = memory_blocks;

    while (current_block) {
        if (current_block->status == 0 && current_block->size >= size) {
            // Found a free block that is large enough
            if (current_block->size > size + sizeof(struct MemoryBlock)) {
                // Split the block if it's larger than needed
                struct MemoryBlock* next_block = (struct MemoryBlock*)((char*)current_block + size + sizeof(struct MemoryBlock));
                next_block->size = current_block->size - size - sizeof(struct MemoryBlock);
                next_block->status = 0; // Free
                next_block->memory = (char*)next_block + sizeof(struct MemoryBlock);

                current_block->size = size;
            }

            current_block->status = 1; // Allocated
            return current_block->memory;
        }

        current_block = (struct MemoryBlock*)((char*)current_block + current_block->size + sizeof(struct MemoryBlock));
    }

    // No suitable block found
    return NULL;
}

void myFree(void* ptr) {
    if (!ptr) {
        // Nothing to free
        return;
    }

    struct MemoryBlock* current_block = memory_blocks;

    while (current_block) {
        if (current_block->memory == ptr) {
            // Mark the block as free
            current_block->status = 0;

            // Check for contiguous free blocks and merge them
            struct MemoryBlock* next_block = (struct MemoryBlock*)((char*)current_block + current_block->size + sizeof(struct MemoryBlock));

            while (next_block && next_block->status == 0) {
                current_block->size += next_block->size + sizeof(struct MemoryBlock);
                next_block = (struct MemoryBlock*)((char*)current_block + current_block->size + sizeof(struct MemoryBlock));
            }

            return;
        }

        current_block = (struct MemoryBlock*)((char*)current_block + current_block->size + sizeof(struct MemoryBlock));
    }
}
