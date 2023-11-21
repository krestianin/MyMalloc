#include "my_malloc.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    initializeMemory();


    printf("Allocating and deallocating in order...\n");
    void* block1 = my_malloc(100);
    if (block1) printf("Allocated 100 bytes for block1\n");
    void* block2 = my_malloc(50);
    if (block2) printf("Allocated 50 bytes for block2\n");
    
    my_free(block2);
    printf("Freed block2\n");
    my_free(block1);
    printf("Freed block1\n");


    printf("\nAllocating and deallocating out of order...\n");
    block1 = my_malloc(30);
    if (block1) printf("Allocated 30 bytes for block1\n");
    block2 = my_malloc(40);
    if (block2) printf("Allocated 40 bytes for block2\n");
    void* block3 = my_malloc(20);
    if (block3) printf("Allocated 20 bytes for block3\n");

    my_free(block2);
    printf("Freed block2 (out of order)\n");
    my_free(block1);
    printf("Freed block1\n");
    my_free(block3);
    printf("Freed block3\n");


    printf("\nTesting allocation when the stack is full...\n");
    void* block4 = my_malloc(MEMORY_SIZE-76); // Attempt to allocate the entire pool
    if (block4) {
        printf("Allocated MEMORY_SIZE bytes for block4\n");
        my_free(block4);
        printf("Freed block4\n");
    } else {
        printf("Failed to allocate MEMORY_SIZE bytes for block4\n");
    }
    void* block5 = my_malloc(39);


    if (block5) {
        printf("Allocated MEMORY_SIZE bytes for block5\n");
        my_free(block5);
        printf("Freed block5\n");
    } else {
        printf("Failed to allocate 100 bytes for block5\n");
    }

    cleanupMemory();

    return 0;
}
